  /**
   * Genera los primeros `count` términos de una serie de Fibonacci.
   * @param {number} count — Número de términos a generar (entero >= 1).
   * @param {number} [first=0] — Valor del primer término (por defecto 0).
   * @param {number} [second=1] — Valor del segundo término (por defecto 1).
   * @returns {number[]} Array con la serie de Fibonacci.
   */
  function generateFibonacci(count, first = 0, second = 1) {
    // Validación básica
    if (!Number.isInteger(count) || count < 1) {
      throw new Error('El parámetro "count" debe ser un entero mayor o igual a 1');
    }

    // Caso base: 1 término → [first]
    if (count === 1) return [first];
    // Caso base: 2 términos → [first, second]
    if (count === 2) return [first, second];

    const result = [first, second];
    let prev = first;
    let curr = second;

    for (let i = 3; i <= count; i++) {
      const next = prev + curr;
      result.push(next);
      // Desplazamos los dos últimos
      prev = curr;
      curr = next;
    }

    return result;
  }
