class Fibonacci {
  constructor() {
    this.memo = {};
  }

  calculate(n) {
    if (n <= 1) return n;
    if (this.memo[n]) return this.memo[n];
    this.memo[n] = this.calculate(n - 1) + this.calculate(n - 2);
    return this.memo[n];
  }
}