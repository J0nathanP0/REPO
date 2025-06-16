class GreedyAnimator {
  constructor(svgEl, controls) {
    this.svg = svgEl;
    this.controls = controls;
    this.steps = [];
    this.currentStep = 0;
    this.timer = null;
  }

  init(amount, coins) {
    // 1. Prepara los datos y dibuja estado inicial
    this.amount = amount;
    this.coins = coins.sort((a,b)=>b-a);
    this.steps = this.recordSteps();
    this.currentStep = 0;
    this.drawInitial();
  }

  recordSteps() {
    const steps = [];
    let rem = this.amount;
    for (const c of this.coins) {
      const cnt = Math.floor(rem / c);
      if (cnt>0) {
        steps.push({ coin: c, count: cnt, remBefore: rem });
        rem -= c*cnt;
      }
    }
    return steps;
  }

  drawInitial() {
    // Limpia y dibuja monedas grises en fila
    this.svg.innerHTML = '';
    // ... por cada denominación crea un grupo <g> con texto
  }

  animate() {
    const delay = +this.controls.speed.value;
    const step = this.steps[this.currentStep++];
    if (!step) { clearInterval(this.timer); return; }
    // 1. Resalta la denominación step.coin
    // 2. Anima la “caída” de step.count monedas a un “cajón” de solución
    // 3. Actualiza texto de resto (step.remBefore → step.remBefore - step.coin*step.count)
  }

  start() {
    if (this.timer) clearInterval(this.timer);
    this.timer = setInterval(()=> this.animate(), +this.controls.speed.value);
  }

  reset() {
    clearInterval(this.timer);
    this.currentStep = 0;
    this.drawInitial();
  }
}

// --------- Inicialización de la app ----------

window.addEventListener('DOMContentLoaded', () => {
  const svg = document.getElementById('viz');
  const controls = {
    amount: document.getElementById('amount'),
    coins : document.getElementById('coins'),
    startBtn: document.getElementById('startBtn'),
    resetBtn: document.getElementById('resetBtn'),
    speed: document.getElementById('speed'),
  };

  const animator = new GreedyAnimator(svg, controls);

  function setup() {
    const amt = parseInt(controls.amount.value, 10);
    const coins = controls.coins.value.split(',').map(n=>+n);
    animator.init(amt, coins);
  }

  controls.startBtn.addEventListener('click', () => {
    if (animator.currentStep === 0) setup();
    animator.start();
  });
  controls.resetBtn .addEventListener('click', () => animator.reset());
});
