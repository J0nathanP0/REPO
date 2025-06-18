function generateFibonacci(count, first = 0, second = 1) {
  if (!Number.isInteger(count) || count < 1) {
    throw new Error('El parámetro "count" debe ser un entero mayor o igual a 1');
  }
  if (count === 1) return [first];
  if (count === 2) return [first, second];

  const result = [first, second];
  let prev = first, curr = second;

  for (let i = 3; i <= count; i++) {
    const next = prev + curr;
    result.push(next);
    prev = curr;
    curr = next;
  }
  return result;
}

function calcularFibonacci() {
  // Obtener la entrada del usuario
  const entrada = document.getElementById('indicesInput').value;

  // Validar y parsear los índices ingresados
  const indices = entrada.split(',')
    .map(s => parseInt(s.trim(), 10))
    .filter(n => !Number.isNaN(n));

  if (indices.length === 0) {
    alert('No se ingresó ningún índice válido.');
    return;
  }

  // Calcular la serie hasta el índice más grande
  const maxIndex = Math.max(...indices);
  const serie = generateFibonacci(maxIndex + 1);

  // Mostrar la secuencia completa de Fibonacci
  const fibonacciList = document.getElementById('fibonacciSequence');
  fibonacciList.innerHTML = ''; // Limpiar la lista antes de mostrar la nueva secuencia

  serie.forEach(value => {
    const listItem = document.createElement('li');
    listItem.textContent = value;
    fibonacciList.appendChild(listItem);
  });

  // Mostrar los resultados en la tabla
  const resultadosTable = document.getElementById('resultados');
  resultadosTable.innerHTML = ''; // Limpiar la tabla antes de mostrar los resultados

  // Agregar encabezados a la tabla
  const encabezado = document.createElement('tr');
  encabezado.innerHTML = '<th>Índice</th><th>Valor de Fibonacci</th>';
  resultadosTable.appendChild(encabezado);

  // Agregar cada fila con los resultados
  indices.forEach(i => {
    const fila = document.createElement('tr');
    fila.innerHTML = `<td>${i}</td><td>${serie[i]}</td>`;
    resultadosTable.appendChild(fila);
  });
}
  