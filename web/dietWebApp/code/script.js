// elementi index.html
const nR = [
  document.getElementById("nomeRicetta1"),
  document.getElementById("nomeRicetta2"),
  document.getElementById("nomeRicetta3"),
];
const iR = [
  document.getElementById("ingredientiRicetta1"),
  document.getElementById("ingredientiRicetta2"),
  document.getElementById("ingredientiRicetta3"),
];
const pR = [
  document.getElementById("preparazioneRicetta1"),
  document.getElementById("preparazioneRicetta2"),
  document.getElementById("preparazioneRicetta3"),
];

const cards = document.querySelectorAll(".recipe-card");

// Funzione principale per ottenere i dati
async function getData() {
  try {
    let response = await fetch("../assets/data.json");
    let data = await response.json();

    // seleziono gli elementi dei rispettivi tipi
    let t1 = data.filter((r) => r.Tipo === 1);
    let t2 = data.filter((r) => r.Tipo === 2);

    if (t1.length < 1 || t2.length < 2) {
      throw new Error("Dati insufficienti");
    }

    let r1 = t1[Math.floor(Math.random() * t1.length)]; // ricetta tipo 1

    // ottengo due ricette con indici diversi e con proteine diverse (ingredienti[0])
    let indexA = Math.floor(Math.random() * t2.length);
    let indexB;
    let counter = 0;
    do {
      indexB = Math.floor(Math.random() * t2.length);
      counter++;
    } while (
      (indexA === indexB ||
        t2[indexA].Ingredienti[0] === t2[indexB].Ingredienti[0]) &&
      counter < 100 // loop infinito se avessimo ricette con solo le stesse proteine, guardia
    );
    if (counter === 100) {
      throw new Error("Dati insufficienti");
    }

    let ricetteScelte = [r1, t2[indexA], t2[indexB]];

    // compilo i campi della pagina
    for (let i = 0; i < 3; i++) {
      let rR = ricetteScelte[i];
      nR[i].innerText = rR.Nome;
      iR[i].innerHTML = rR.Ingredienti.map((ing) => `<li>${ing}</li>`).join(""); // piu' efficiente che un ciclo for a se
      pR[i].innerHTML =
        `<h2>Preparazione Ricetta:</h2> <p>${rR.Preparazione}</p>`;

      // Gemini: Estetica Animazione Ricette
      cards[i].classList.remove("fade-in");
      void cards[i].offsetWidth;
      cards[i].classList.add("fade-in");
    }
  } catch (e) {
    console.error(e);
    for (let i = 0; i < 3; i++) {
      nR[i].innerText = "NOT FOUND";
      iR[i].innerHTML = "";
      pR[i].innerHTML = `<p>Errore nella generazione della Ricetta</p>`;
    }
  }
}

// Event listener per il pulsante
let b = document.getElementById("b");
b.addEventListener("click", (event) => {
  event.target.blur();
  for (let i = 0; i < 3; i++) {
    pR[i].innerHTML = `<p>Generando Ricetta Casuale ...</p>`;
    nR[i].innerText = "Attendere...";
    iR[i].innerHTML = "";
  }
  getData(); // richiamo la funzione principale
});
