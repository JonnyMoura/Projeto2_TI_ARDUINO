
long tempo_de_jogo = 90000;
bool feito = false;
int valor = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  Serial.begin(9600);
  randomSeed(analogRead(A5));

}
///////Funções globais


int leitor_porta_serie() {
  if (Serial.available() > 0) {
    String a = Serial.readStringUntil("\n");
    valor = a.toInt();
    feito = true;
    Serial.println("Valor lido:" + String(valor, BIN));
  }
  return valor;
}


////Efetuar operações
int operacoes(int operacao, byte primeiro_numero, int porta_serie) {
  if (operacao == 2) {
    primeiro_numero = primeiro_numero ^ porta_serie;
    return (primeiro_numero);

  }
  if (operacao == 4) {
    primeiro_numero = primeiro_numero & porta_serie;
    return (primeiro_numero);
  }
  if (operacao == 3) {
    primeiro_numero = primeiro_numero | porta_serie;
    return (primeiro_numero);
  }


}
//////Mostrador de tempo
void leds(unsigned long timer) {
  if ((millis() - timer) > (tempo_de_jogo) * (1.0 / 4)) {
    digitalWrite(8, HIGH);
  }
  if ((millis() - timer) > ((tempo_de_jogo) * (1.0 / 2))) {
    digitalWrite(9, HIGH);
  }
  if ((millis() - timer) > ((tempo_de_jogo) * (3.0 / 4))) {
    digitalWrite(10, HIGH);
  }
  if ((millis() - timer) >= ((tempo_de_jogo))) {
    digitalWrite(11, HIGH);
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  bool vitoria = false;
  unsigned long timer = millis();
  //gerador aleatorio
  byte numero_inicial = random(1, 256);
  byte target = random(1, 256);
  int botoes_disponiveis = random(1, 4);

  if (botoes_disponiveis == 1) {
    Serial.println("Operações disponíveis:XOR-Vermelho");
  }
  if (botoes_disponiveis == 2) {
    Serial.println("Operações disponíveis:AND-Branco");
  }
  if (botoes_disponiveis == 3) {
    Serial.println("Operações disponíveis:XOR-Vermelho,AND-Branco");
  }
  Serial.println("Valor inicial:" + String(numero_inicial, BIN));
  Serial.println("Target:" + String(target, BIN));
  Serial.println("Introduz um valor:");
  while ((millis() - timer) < tempo_de_jogo )/////Inicio do round
  {
    leds(timer);

    int valor_serie = leitor_porta_serie();

    if (feito == true) {
      ////Operações a executar(verificação de botões ativos)
      if (digitalRead(3) == LOW) {
        numero_inicial = operacoes(3, numero_inicial, valor_serie);
        Serial.println("Valor calculado:" + String(numero_inicial, BIN));
        feito = false;
        Serial.println("Introduz um valor:");

      }
      else if (digitalRead(2) == LOW && (botoes_disponiveis == 1 || botoes_disponiveis == 3)) {
        numero_inicial = operacoes(2, numero_inicial, valor_serie);
        Serial.println("Valor calculado:" + String(numero_inicial, BIN));
        feito = false;
        Serial.println("Introduz um valor:");
      }
      else if (digitalRead(4) == LOW && (botoes_disponiveis == 2 || botoes_disponiveis == 3)) {
        numero_inicial = operacoes(4, numero_inicial, valor_serie);
        Serial.println("Valor calculado:" + String(numero_inicial, BIN));
        feito = false;
        Serial.println("Introduz um valor:");
      }
      if (numero_inicial == target) {////Vitória!!
        Serial.println("GANHOU");
        vitoria = true;
        break;
      }
    }
  }
  if (vitoria == false) {
    Serial.println("Perdeu");
  }
  for (int i = 8; i <= 11; i++) {
    digitalWrite(i, LOW);
  }
}
