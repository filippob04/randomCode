// Loading del documento statico
document.addEventListener("DOMContentLoaded", () => {
  document.getElementById("header-name").innerText =
    masterProfile.personal.name;
  document.getElementById("profile-initials").innerText =
    masterProfile.personal.initials;
  document.getElementById("header-role").innerText =
    masterProfile.personal.role;
  document.getElementById("header-summary").innerText =
    masterProfile.personal.baseSummary;

  const contactList = document.getElementById("contact-list");
  let contactsHTML = "";
  masterProfile.personal.contacts.forEach((c) => {
    contactsHTML += `<div>${c.value}</div>`;
  });
  contactList.innerHTML = contactsHTML;

  const eduContainer = document.getElementById("education-container");
  let eduHTML = "";
  masterProfile.education.forEach((e) => {
    eduHTML += `
        <div class="entry">
            <div class="entry-header">
                <div class="entry-title">${e.degree}</div>
                <div class="entry-date">${e.date}</div>
            </div>
            <div class="entry-subtitle">${e.institution}</div>
            <div class="entry-details">${e.details}</div>
        </div>`;
  });
  eduContainer.innerHTML = eduHTML;

  const langContainer = document.getElementById("languages-list");
  let langHTML = "";
  masterProfile.languages.forEach((l) => {
    langHTML += `<div>${l}</div>`;
  });
  langContainer.innerHTML = langHTML;
});

// Esecuzione
const btn = document.getElementById("submit");
btn.addEventListener("click", generateCV);

// Funzione principale
async function generateCV() {
  const apiKey = document.getElementById("apiKey").value;
  const jobDesc = document.getElementById("jobDescription").value;
  const status = document.getElementById("status");

  if (!apiKey || !jobDesc) {
    alert("Compila tutti i Campi!");
    return;
  }

  btn.disabled = true;
  btn.classList.add("loading");
  btn.innerText = "Analisi in corso...";
  status.innerText = "Processing...";

  // Prompt API
  const profileForAI = { ...masterProfile };
  delete profileForAI.personal.contacts;

  const promptText = `
      Sei un Analista Dati rigoroso per il recruiting. Il tuo compito è filtrare i dati di un candidato per creare un CV mirato, basandoti ESCLUSIVAMENTE sui fatti forniti.
      
      JOB DESCRIPTION (JD):
      ${jobDesc}
  
      MASTER DATA CANDIDATO (Fonte di Verità):
      ${JSON.stringify(profileForAI)}
  
      REGOLE INDEROGABILI (ANTI-ALLUCINAZIONE):
      1. INTERSEZIONE RIGIDA: Seleziona skill e progetti dal MASTER DATA *solo se* sono pertinenti alla JD.
      2. DIVIETO DI INVENZIONE: Se la JD richiede una competenza (es. "React") che NON è presente esplicitamente nel MASTER DATA, NON INSERIRLA. Non inferire competenze non scritte.
      3. FEDELTÀ TECNICA: Non cambiare i nomi delle tecnologie (es. non trasformare "Java" in "Spring Boot" se non è specificato).
      4. SUMMARY EVIDENCE-BASED: Scrivi un summary in prima persona che colleghi le skill verificate del candidato ai requisiti della JD. Non usare aggettivi superlativi (es. "esperto", "visionario") se non supportati dai dati.
      5. ROLE: Usa il Job Title della JD solo se il candidato ha le skill per sostenerlo, altrimenti usa un titolo onesto come "Junior [Tecnologia] Developer" o "CS Student".
      
      FORMATO JSON RICHIESTO:
      {
          "role": "Titolo professionale onesto e mirato",
          "summary": "Bio professionale di max 3 frasi basata sui fatti",
          "skills": ["Skill1", "Skill2", ... (Solo quelle presenti nel Master Data e utili per la JD)],
          "projects": [ 
              { 
                  "name": "Nome Progetto", 
                  "tech": "Stack Tecnologico", 
                  "desc": "Descrizione che evidenzia l'uso delle tecnologie richieste nella JD (senza inventare funzionalità)" 
              } 
          ],
          "experiences": [ 
              { "name": "...", "tech": "...", "desc": "..." } 
          ]
      }
      
      Restituisci SOLO il JSON valido.
    `;

  if (!promptText) {
    status.innerText = "Errore creazione prompt";
    return;
  }

  // Usa l'alias "latest" che Google assegna automaticamente al modello funzionante per l'account
  const url = `https://generativelanguage.googleapis.com/v1beta/models/gemini-flash-latest:generateContent?key=${apiKey}`; // Gemini
  try {
    const response = await fetch(url, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({
        contents: [
          {
            parts: [{ text: promptText }],
          },
        ],
        generationConfig: {
          responseMimeType: "application/json",
        },
      }),
    });

    const data = await response.json();
    if (data.error) {
      console.error("ERRORE API:", data.error);
      throw new Error(data.error.message || "Errore sconosciuto da Google");
    }

    // Pulizia output llm
    let content = data.candidates[0].content.parts[0].text;
    const tailoredCV = JSON.parse(content);

    populateHTML(tailoredCV); // Funzione di popolamento

    status.innerText = "CV Generato!";
  } catch (error) {
    console.error(error);
    alert("Errore: " + error.message);
    status.innerText = "Errore durante la generazione.";
  } finally {
    btn.classList.remove("loading");
    btn.disabled = false;
    btn.innerText = "Genera CV";
  }
}

// Funzione di Popolamento index.html
function populateHTML(data) {
  document.getElementById("header-role").innerText = data.role;
  document.getElementById("header-summary").innerText = data.summary;

  const skillsList = document.getElementById("hard-skills-list");
  let skillsHTML = "";
  (data.skills || []).forEach((s) => {
    skillsHTML += `<li>${s}</li>`;
  });
  skillsList.innerHTML = skillsHTML;

  const projContainer = document.getElementById("projects-container");
  let projectsHTML = "";
  (data.projects || []).forEach((p) => {
    projectsHTML += `
        <div class="entry">
            <div class="entry-header">
                <div class="entry-title">${p.name}</div>
                <div class="tech-stack">${p.tech}</div>
            </div>
            <div class="entry-details">${p.desc}</div>
        </div>`;
  });
  projContainer.innerHTML = projectsHTML;

  const expContainer = document.getElementById("experience-container");
  let expHTML = "";
  (data.experiences || []).forEach((p) => {
    expHTML += `
        <div class="entry">
            <div class="entry-header">
                <div class="entry-title">${p.name}</div>
                <div class="tech-stack">${p.tech}</div>
            </div>
            <div class="entry-details">${p.desc}</div>
        </div>`;
  });
  expContainer.innerHTML = expHTML;
}
