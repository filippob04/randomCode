let display = document.getElementById("display");
display.textContent = "0";

let chrono = [];

let currentOperator = null;
let firstOperand = null;
let waitingForSecondOperand = false;

function inputDigit(digit) {
  if (waitingForSecondOperand === true) {
    display.textContent += digit;
    waitingForSecondOperand = false;
  } else {
    if (display.textContent === "0") {
      display.textContent = digit;
    } else {
      display.textContent += digit;
    }
  }
}
function inputDecimal(dot) {
  if (waitingForSecondOperand === true) {
    if (display.textContent.includes(currentOperator)) {
      display.textContent += "0" + dot;
    } else {
      display.textContent = "0" + dot;
    }
    waitingForSecondOperand = false;
    return;
  }

  const parts = display.textContent.split(currentOperator || "N/A");
  const currentNumber = parts[parts.length - 1].trim();

  if (!currentNumber.includes(dot)) {
    display.textContent += dot;
  }
}
function handleOperator(nextOperator) {
  const currentDisplayValue = display.textContent;

  const parts = currentDisplayValue.split(currentOperator || "N/A");
  const currentNumberString = parts[parts.length - 1].trim();
  const inputValue = parseFloat(currentNumberString);
  if (isNaN(inputValue) && firstOperand === null) {
    return;
  }

  if (firstOperand === null) {
    firstOperand = inputValue;
  } else if (currentOperator === null && !waitingForSecondOperand) {
    firstOperand = inputValue;
  } else if (currentOperator && !waitingForSecondOperand) {
    const operandA = firstOperand;
    const operandB = inputValue;
    const operator = currentOperator;

    const result = calculate(operandA, operandB, operator);

    if (isNaN(result)) {
      resetCalculator();
      return;
    }

    const partialResultString = `${operandA} ${operator} ${operandB} = ${result}`;
    chrono.push(partialResultString + "; ");

    firstOperand = result;
  }

  waitingForSecondOperand = true;
  currentOperator = nextOperator;

  display.textContent = `${firstOperand} ${currentOperator} `;
}
function calculate(numA, numB, operator) {
  if (isNaN(numA) || isNaN(numB)) {
    alert("Input non valido. Inserisci solo numeri.");
    return NaN;
  }
  let res;
  switch (operator) {
    case "+":
      res = numA + numB;
      break;
    case "-":
      res = numA - numB;
      break;
    case "×":
      res = numA * numB;
      break;
    case "÷":
      if (numB === 0) {
        alert("Non si puo' dividere per zero!");
        return NaN;
      }
      res = numA / numB;
      break;
    case "^":
      if (numA < 0 && Number.isInteger(1 / numB) && (1 / numB) % 2 === 0) {
        alert("Non si puo' eseguire una radice pari di un numero negativo");
        return NaN;
      }
      res = numA ** numB;
      break;
    case "%":
      if (numB === 0) {
        alert("Non si puo' calcolare il modulo per zero!");
        return NaN;
      }
      res = numA % numB;
      break;
    default:
      return numB;
  }
  return res;
}
function handleEquals() {
  if (currentOperator === null || waitingForSecondOperand) {
    return;
  }
  const parts = display.textContent.split(currentOperator);
  const secondOperandString =
    parts.length > 1 ? parts[1].trim() : display.textContent;

  if (secondOperandString === "") {
    return;
  }
  const secondOperand = parseFloat(secondOperandString);
  const result = calculate(firstOperand, secondOperand, currentOperator);

  if (isNaN(result)) {
    resetCalculator();
    return;
  }

  display.textContent = String(result);

  const resultString = `${firstOperand} ${currentOperator} ${secondOperand} = ${result}`;
  chrono.push(resultString + "; ");

  firstOperand = result;
  currentOperator = null;
  waitingForSecondOperand = false;
}
// Gestisce le cancellazioni degli operandi, non dei singoli numeri per come sono gestiti first operand e second operand
function handleCanc() {
  const DisplayValue = display.textContent;
  // Se la calcolatrice e' vuota
  if (DisplayValue === "0") {
    return;
  }
  // Se sto aspettando un secondo operando
  if (waitingForSecondOperand == true) {
    display.textContent = String(firstOperand);
    currentOperator = null;
    waitingForSecondOperand = false;
    return;
  }
  // Se non ho cancora premuto =
  if (currentOperator !== null && waitingForSecondOperand === false) {
    const parts = DisplayValue.split(currentOperator);
    const secondOperandPart = parts[1].trim();

    if (secondOperandPart !== "") {
      display.textContent = `${firstOperand} ${currentOperator} `;
      waitingForSecondOperand = true;
      return;
    }
  }
  resetCalculator();
}
function resetCalculator() {
  display.textContent = "0";
  firstOperand = null;
  currentOperator = null;
  waitingForSecondOperand = false;
}
function showChrono() {
  if (chrono.length === 0) {
    alert("Nessuna operazione nello storico.");
    return;
  }
  alert("Storico:\n" + chrono.join("\n"));
  chrono = [];
}

for (let i = 0; i <= 9; i++) {
  const button = document.getElementById(String(i));
  if (button) {
    button.addEventListener("click", () => inputDigit(String(i)));
  }
}
document
  .getElementById("decimal")
  .addEventListener("click", () => inputDecimal("."));
document
  .getElementById("sum")
  .addEventListener("click", () => handleOperator("+"));
document
  .getElementById("sub")
  .addEventListener("click", () => handleOperator("-"));
document
  .getElementById("mult")
  .addEventListener("click", () => handleOperator("×"));
document
  .getElementById("divi")
  .addEventListener("click", () => handleOperator("÷"));
document
  .getElementById("pow")
  .addEventListener("click", () => handleOperator("^"));
document
  .getElementById("mod")
  .addEventListener("click", () => handleOperator("%"));
document.getElementById("canc").addEventListener("click", handleCanc);
document.getElementById("equals").addEventListener("click", handleEquals);
document.getElementById("chrono").addEventListener("click", showChrono);
document.getElementById("ac").addEventListener("click", resetCalculator);
