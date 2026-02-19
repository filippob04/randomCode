# 📄 AI Smart CV Generator

Un'applicazione web **client-side** che genera automaticamente un Curriculum Vitae su misura analizzando una Job Description. Utilizza l'intelligenza artificiale di **Google Gemini** per selezionare le competenze più rilevanti e riscrivere il sommario, garantendo un CV ottimizzato per ogni candidatura.

## 🚀 Caratteristiche Principali

- **🎯 Adattamento Contestuale:** Analizza la _Job Description_ e seleziona dal tuo "Master Profile" solo i progetti e le skill pertinenti.
- **🤖 Integrazione Google Gemini:** Sfrutta il modello `gemini-2.0-flash` per un'elaborazione rapida e gratuita.
- **🛡️ Logica Anti-Allucinazione:** Il prompt è ingegnerizzato per impedire all'AI di inventare competenze non possedute (Strict Intersection Logic).
- **🖨️ Print-Ready Design:** Layout CSS ottimizzato per la stampa su foglio A4, con stile moderno e pulito (Clean Tech).
- **💻 Terminal UI:** Pannello di controllo in stile "Hacker/Retro Terminal" per inserire API Key e Job Description.
- **🔒 Privacy First:** Funziona interamente nel browser. La tua API Key e i tuoi dati non vengono mai salvati su un database esterno.

## 🛠️ Tecnologie Utilizzate

- **HTML5** - Struttura semantica.
- **CSS3** - Variabili CSS, Flexbox/Grid, @media print per l'esportazione PDF.
- **JavaScript (Vanilla)** - Logica DOM, chiamate asincrone `fetch` all'API.
- **Google Gemini API** - Motore di generazione del contenuto.

## 📦 Installazione e Utilizzo

Non è necessaria alcuna installazione (niente Node.js o NPM). Il progetto è "plug-and-play".

1. **Clona la repository:**

```bash
git clone https://github.com/tuo-username/ai-cv-generator.git

```

2. **Configura i tuoi dati:**

- Apri il file `data.js`.
- Modifica l'oggetto `masterProfile` inserendo i tuoi dati reali (Esperienze, Progetti, Skill, Contatti).

3. **Avvia l'app:**

- Apri semplicemente il file `index.html` con il tuo browser preferito (Chrome, Edge, Firefox).

4. **Genera il CV:**

- Ottieni una **API Key Gratuita** da [Google AI Studio](https://aistudio.google.com/app/apikey).
- Incolla la Key nel pannello di controllo.
- Incolla la _Job Description_ dell'offerta di lavoro.
- Clicca su **Genera CV**.

5. **Esporta:**

- Premi `CTRL + P` (o CMD + P) e salva come **PDF**.

## 📂 Struttura del Progetto

```text
/
├── index.html      # Struttura della pagina
├── style.css       # Stili per A4, Sidebar e Terminale
├── script.js       # Logica API e gestione DOM
├── data.js         # Il tuo "Database" personale (Master Data)
└── README.md       # Documentazione

```

## 🧠 Come funziona il Prompt Engineering

Per evitare che l'AI "inventi" competenze per compiacere il recruiter, il sistema utilizza un **System Prompt rigoroso** che impone:

1. **Intersezione Rigida:** Usa solo skill presenti nel Master Data.
2. **Evidence Based:** Il summary deve basarsi solo su fatti comprovati.
3. **No Inference:** Non deduce framework non esplicitamente listati (es. se sai Java, non assume che tu sappia Spring Boot a meno che non sia scritto).

## ✨ Personalizzazione

- **Design:** Modifica le variabili CSS in `style.css` (es. `--accent-col`) per cambiare la palette colori.
- **Modello AI:** In `script.js` puoi cambiare la versione del modello (es. da `gemini-1.5-flash` a `gemini-pro`).

---

**Anteprima:** [Filippo Baldini](https://github.com/filippob04/filippob04/blob/main/cv.pdf)
