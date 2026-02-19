# 🎵 Music Recognition App
## 🚀 Funzionalità

- **🎤 Registrazione audio**: Registra 10 secondi di audio dal microfono e salva il risultato in un file WAV.
- **🎶 Riconoscimento musicale**: Utilizza l'API di [AudD](https://audd.io/) per identificare il brano registrato.
- **💿 Integrazione con Spotify**: Recupera la copertina dell'album del brano identificato utilizzando l'API di [Spotify](https://developer.spotify.com/documentation/web-api).
- **🖼️ Visualizzazione fullscreen**: Mostra la copertina dell'album a schermo intero utilizzando OpenCV.

## 📦 Dipendenze

- [PortAudio](http://www.portaudio.com/) - Per la registrazione audio.
- [libsndfile](http://www.mega-nerd.com/libsndfile/) - Per la gestione dei file WAV.
- [libcurl](https://curl.se/libcurl/) - Per le richieste HTTP alle API.
- [JSONCPP](https://github.com/open-source-parsers/jsoncpp) - Per il parsing delle risposte JSON.
- [OpenCV](https://opencv.org/) - Per la visualizzazione dell'immagine a schermo intero.
- [OpenSSL](https://www.openssl.org/) - Per la codifica Base64 (necessaria per l'autenticazione con Spotify).

## 💻 Installazione

### macOs

```zsh
    brew install portaudio
    brew install libsndfile
    brew install jsoncpp
    brew install openssl
    brew install opencv
    brew install curl
```

## ⚙️ Come Eseguire:

```zsh
make
```

## 🧰 Configurazione

Prima di eseguire il programma, assicurarsi di avere:

- Una chiave API valida per AudD. Si sostituisca il campo mancante nel codice con la propria chiave.
- Un account sviluppatore Spotify e le credenziali Client ID e Client Secret. Si modifichino dunque i campi marcati con le credenziali personali.
- Eventualmente modificare i dispositivi di input e la grandezza dello schermo per l'output
  
## ▶️ Esecuzione

Dopo aver compilato con il makefile fornito eseguire separatamente con ./recorder e ./send
- Il programma registrerà 10 secondi di audio dal microfono predefinito.
- Identificherà il brano utilizzando audD.
- Recupererà la copertina dell'album da Spotify.
- Visualizzerà la copertina a schermo intero.
- Infine si prema un tasto qualsiasi per chiudere la finestra della copertina.

### 📄 Note

Assicurarsi di avere un microfono funzionante collegato al sistema.
Il programma è stato testato su Linux. Potrebbero essere necessarie modifiche per altri sistemi operativi.
Le credenziali API nel codice sono esempi. Devono essere sostituire con le proprie credenziali personali.

- ✉️ [Baldini Filippo](mailto:6393212@studenti.unige.it)

