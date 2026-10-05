/*
  ======================================================================
  Servidor Web Básico — ESP32 (modo Station / conectado ao Wi-Fi local)
  ======================================================================
  Objetivo: o ESP32 conecta na sua rede Wi-Fi (a mesma do seu PC/celular)
  e hospeda uma página HTML simples, acessível de qualquer dispositivo
  na mesma rede, digitando o IP do ESP32 no navegador.

  Este é o passo intermediário pedido pelo professor — depois que isso
  estiver funcionando, é só substituir o conteúdo HTML fixo pelos
  valores reais lidos do BME680 (próxima etapa do projeto).

  BIBLIOTECAS NECESSÁRIAS:
  - Nenhuma instalação extra! WiFi.h e WebServer.h já vêm inclusas
    quando você instala o "ESP32 by Espressif Systems" no Boards Manager
    do Arduino IDE (que você já tem, já que o teste anterior funcionou).

  IMPORTANTE:
  - Preencha SSID e SENHA da sua rede Wi-Fi abaixo antes de fazer upload.
  - Seu PC/celular precisa estar NA MESMA REDE Wi-Fi que o ESP32 pra
    conseguir acessar a página (não funciona entre redes diferentes,
    tipo Wi-Fi de casa vs. dados móveis).
  - Redes Wi-Fi de faculdade/escola com portal de login (aquelas que
    pedem usuário/senha numa página depois de conectar) geralmente NÃO
    funcionam com o ESP32 — ele não sabe preencher esse tipo de login.
    Se estiver numa rede assim, use o Wi-Fi de um celular (hotspot) para
    testar.
  ======================================================================
*/

#include <WiFi.h>
#include <WebServer.h>

// ---------------------- CONFIGURAÇÃO DE WI-FI ----------------------
const char* WIFI_SSID = "Visitantes";     // <-- troque aqui
const char* WIFI_SENHA = "";   // <-- troque aqui

// Servidor web na porta 80 (porta padrão HTTP, o navegador usa ela
// automaticamente sem precisar digitar o número da porta na URL)
WebServer servidor(80);

// ======================================================================
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("=== Servidor Web ESP32 ===");

  // Conecta na rede Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_SENHA);
  Serial.print("Conectando ao Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi conectado com sucesso!");
  Serial.print("Endereço IP do ESP32: ");
  Serial.println(WiFi.localIP());
  Serial.println("Digite esse IP no navegador de um dispositivo");
  Serial.println("conectado na MESMA rede Wi-Fi pra abrir a página.");
  Serial.println("===========================");

  // Define qual função responde quando alguém acessa a página inicial ("/")
  servidor.on("/", tratarPaginaInicial);

  // Define uma resposta para qualquer caminho não encontrado (erro 404)
  servidor.onNotFound(tratarNaoEncontrado);

  servidor.begin();
  Serial.println("Servidor HTTP iniciado.");
}

// ======================================================================
void loop() {
  // Essencial: precisa ser chamado constantemente para o servidor
  // processar as requisições que chegam
  servidor.handleClient();
}

// ======================================================================
// Função chamada toda vez que alguém acessa http://<IP-do-ESP32>/
void tratarPaginaInicial() {
  String html = R"HTML(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Air3D Guard</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #1a1a2e;
      color: #eaeaea;
      text-align: center;
      padding-top: 50px;
    }
    h1 {
      color: #4ade80;
    }
    .card {
      background-color: #16213e;
      display: inline-block;
      padding: 30px 50px;
      border-radius: 12px;
      margin-top: 20px;
    }
    .status {
      font-size: 24px;
      color: #4ade80;
    }
  </style>
</head>
<body>
  <h1>Air3D Guard</h1>
  <div class="card">
    <p class="status">Servidor funcionando!</p>
    <p>Esta pagina esta sendo hospedada diretamente pelo ESP32.</p>
  </div>
</body>
</html>
)HTML";

  servidor.send(200, "text/html", html);
}

// ======================================================================
// Função chamada quando alguém tenta acessar um caminho que não existe
// (ex: http://<IP>/qualquer-coisa)
void tratarNaoEncontrado() {
  servidor.send(404, "text/plain", "Pagina nao encontrada");
}
