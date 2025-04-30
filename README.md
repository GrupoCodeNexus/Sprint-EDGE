# SmartFlow - Controle de Acesso para Carrinho de Suprimentos Hospitalar

Este repositório contém o projeto de controle de acesso desenvolvido para o carrinho de suprimentos "SmartFlow", uma iniciativa do grupo CodeNexus em parceria com o Hospital Infantil Sabará.

## Projeto

O projeto SmartFlow visa implementar um sistema de controle de acesso inteligente para carrinhos de suprimentos médicos. O objetivo principal é aumentar a segurança e o rastreamento do uso desses carrinhos dentro do ambiente hospitalar para maior controle de acesso e estoque.

## Solução

A solução implementada utiliza um sistema de identificação por aproximação (RFID) para controlar o acesso ao carrinho. Apenas usuários com cartões autorizados podem destravar a gaveta de suprimentos. Cada tentativa de acesso (autorizada ou não) é registrada e enviada para uma plataforma de monitoramento remota, proporcionando visibilidade em tempo real sobre quem acessou o carrinho e quando.

### Diagrama
![Diagrama](/DiagramaCH-Edge-CodeNexus.drawio.png)

## Especificações

**Hardware:**
1. Microcontrolador: **ESP32**
2. Leitor de Cartão: **RFC522**
3. Feedback Visual: **LCD I2C**, LED Verde (Acesso Permitido), LED Vermelho (Acesso Negado)
4. Feedback Sonoro: Buzzer
5. Mecanismo de Travamento: **Servo motor** para acionamento da gaveta

**Máquina Virtual (VM):**
1. Provedor: **Microsoft Azure**
2. Sistema Operacional: **Linux (Ubuntu)**
3. Configuração: **Standard B1s (1 vCPU, 1 GiB de memória)**

**Software/Plataformas:**
1. **FIWARE** (Orion Context Broker, STH-Comet para gerenciamento e histórico de dados)
2. **Wokwi** (Plataforma de simulação para desenvolvimento)
3. **Arduino IDE** (Ambiente de desenvolvimento para o ESP32)
4. **Postman** (Ferramenta para testes de API e comunicação com a VM)

## Links Úteis

Link da Simulação no Wokwi: [Clique aqui](https://wokwi.com/projects/428695509410562049)

Link do Vídeo de Demonstração: [Clique aqui](https://youtu.be/V9G8wEmKn1c?si=8GbYCUWSue7V-Zu3)

## Integrantes da Equipe CodeNexus

- [Francisco Vargas](https://github.com/Franciscov25)
- [Kayque Carvalho](https://github.com/Kay-Carv)
- [Matheus Eiki](https://github.com/Matheus-Eiki)
- [Marcelo Affonso](https://github.com/tenebres-cpu)

**Contextualização:**

O grupo CodeNexus apresenta o SmartFlow, uma solução de controle de acesso inteligente para o carrinho de suprimentos médicos do Hospital Infantil Sabará. Nosso protótipo demonstra um sistema de controle de acesso em tempo real, visando maior visibilidade e controle sobre a utilização dos carrinhos, pavimentando o caminho para futuras implementações de controle de estoque.
