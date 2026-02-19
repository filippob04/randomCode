#include <fstream>
#include <iostream>
#include <sstream>

#include <curl/curl.h>
#include <json/json.h>
#include <opencv2/opencv.hpp>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

// Memorizzazione risposta HTTP in una stringa
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Funzione per Base64 encode (necessario per Spotify Client Credentials)
std::string base64_encode(const std::string &in) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio, in.c_str(), in.length());
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);
    std::string out(bufferPtr->data, bufferPtr->length);
    BIO_free_all(bio);
    return out;
}

// Richiede token Spotify
std::string getSpotifyToken(const std::string &clientId, const std::string &clientSecret){
    std::string auth = clientId + ":" + clientSecret;
    std::string encodedAuth = base64_encode(auth);

    CURL* curl = curl_easy_init();
    if(!curl) return "";

    curl_easy_setopt(curl, CURLOPT_URL, "https://accounts.spotify.com/api/token");
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    struct curl_slist* headers = nullptr;
    std::string authHeader = "Authorization: Basic " + encodedAuth;
    headers = curl_slist_append(headers, authHeader.c_str());
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    std::string postFields = "grant_type=client_credentials";
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());

    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    if(res != CURLE_OK){return "";}

    Json::CharReaderBuilder builder;
    Json::Value root;
    std::stringstream sstr(response);
    std::string errs;
    if(!Json::parseFromStream(builder, sstr, &root, &errs)) return "";

    return root["access_token"].asString();
}

/********************************************************************************************* 
 *************************************** MAIN FUNCTION ***************************************
 *********************************************************************************************/

int main(){

    // API audD per identificare il brano
    const char* filename = "output.wav";
    const char* apiToken = "INSERT KEY"; // chiave API

    CURL* curl = curl_easy_init();
    if(!curl){
        std::cerr << "Errore inizializzazione curl\n";
        return 1;
    }

    curl_mime* form = curl_mime_init(curl);
    if(!form){
        std::cerr << "Errore creazione form\n";
        curl_easy_cleanup(curl);
        return 1;
    }
    
    // File WAV
    curl_mimepart* field = curl_mime_addpart(form);
    curl_mime_name(field, "file");
    curl_mime_filedata(field, filename);

    // API token
    field = curl_mime_addpart(form);
    curl_mime_name(field, "api_token");
    curl_mime_data(field, apiToken, CURL_ZERO_TERMINATED);

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.audd.io/");
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);

    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if(res != CURLE_OK){
        std::cerr << "Errore curl: " << curl_easy_strerror(res) << "\n";
        curl_mime_free(form);
        curl_easy_cleanup(curl);
        return 1;
    } 

    // Gestione risposta JSON
    Json::CharReaderBuilder builder;
    Json::Value root;
    std::stringstream sstr(response);
    std::string errs;
    if(!Json::parseFromStream(builder, sstr, &root, &errs)) {
        std::cerr << "Errore parsing JSON: " << errs << "\n";
        curl_mime_free(form);
        curl_easy_cleanup(curl);
        return 1;
    }
    if(root["result"].isNull()) {
        std::cout << "Brano non trovato\n";
        curl_mime_free(form);
        curl_easy_cleanup(curl);
        return 1;
    }

    std::string artist = root["result"]["artist"].asString();
    std::string title = root["result"]["title"].asString();
    std::cout << "Artista: " << artist << "\nTitolo: " << title << "\n";

    curl_mime_free(form);
    curl_easy_cleanup(curl);

/********************************************************************************************* 
 ***************************************** Spotify *******************************************
 *********************************************************************************************/

    // Spotify Token
    const std::string clientId = "INSERT CID"; // Client ID
    const std::string clientSecret = "INSERT CS"; // Client Secret
    std::string spotifyToken = getSpotifyToken(clientId, clientSecret);
    if(spotifyToken.empty()){
        std::cerr << "Impossibile ottenere token Spotify\n";
        return 1;
    }

    // Spotify Search
    std::string query = artist + " " + title;
    for(auto &c : query) if(c==' ') c='+';
    std::string spotifyUrl = "https://api.spotify.com/v1/search?q=" + query + "&type=track&limit=1";

    curl = curl_easy_init();
    if(!curl){return 1;}

    std::string spotifyResponse;
    curl_easy_setopt(curl, CURLOPT_URL, spotifyUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, ("Authorization: Bearer " + spotifyToken).c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &spotifyResponse);

    if(curl_easy_perform(curl) != CURLE_OK){
        std::cerr << "Errore curl Spotify\n";
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        return 1;
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    // Gestione risposta JSON
    Json::Value spRoot;
    std::stringstream spSstr(spotifyResponse);
    if(!Json::parseFromStream(builder, spSstr, &spRoot, &errs)){
        std::cerr << "Errore parsing JSON Spotify: " << errs << "\n";
        return 1;
    }

    if(!spRoot["tracks"]["items"][0]["album"]["images"][0]["url"].isString()){
        std::cerr << "Cover Spotify non trovata\n";
        return 1;
    }

    std::string coverUrl = spRoot["tracks"]["items"][0]["album"]["images"][0]["url"].asString();
    std::cout << "Cover URL: " << coverUrl << "\n";

    // Album Cover Download
    CURL* imgCurl = curl_easy_init();
    if(!imgCurl){return 1;}
    FILE* imgFile = fopen("cover.jpg", "wb");
    if(!imgFile){return 1;}

    curl_easy_setopt(imgCurl, CURLOPT_URL, coverUrl.c_str());
    curl_easy_setopt(imgCurl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(imgCurl, CURLOPT_WRITEDATA, imgFile);
    if(curl_easy_perform(imgCurl) != CURLE_OK){
        std::cerr << "Errore download cover\n";
        curl_easy_cleanup(imgCurl);
        fclose(imgFile);
        return 1;
    }

    curl_easy_cleanup(imgCurl);
    fclose(imgFile);

/********************************************************************************************* 
 ***************************************** OUTPUT ********************************************
 *********************************************************************************************/

    // OpenCv per immagine a schermo
    cv::Mat image = cv::imread("cover.jpg");
    if(image.empty()){
        std::cerr << "Errore caricamento immagine cover\n";
        return 1;
    }

    // Risoluzione Schermo
    int screenWidth = 2560;
    int screenHeight = 1440;

    // Ridimensiona l'immagine mantenendo il rapporto d'aspetto
    double scale = std::min(screenWidth / (double)image.cols, screenHeight / (double)image.rows);
    cv::Mat resized;
    cv::resize(image, resized, cv::Size(), scale, scale);
    cv::Mat display(screenHeight, screenWidth, resized.type(), cv::Scalar(0,0,0));
    int x = (screenWidth - resized.cols)/2;
    int y = (screenHeight - resized.rows)/2;
    resized.copyTo(display(cv::Rect(x, y, resized.cols, resized.rows)));

    // Mostra l'immagine a schermo intero
    cv::namedWindow("Cover", cv::WINDOW_NORMAL);
    cv::setWindowProperty("Cover", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
    cv::imshow("Cover", display);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
