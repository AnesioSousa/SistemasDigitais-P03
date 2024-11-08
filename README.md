![Tela Inicial do Tetris](./Imagem_do_inicio.jpeg)



# 🟦🟩🟥🟨 GPU-lib 🟩🟨🟥🟦
Biblioteca com funções gráficas para o Processador Gráfico CoLenda, que permitem facilitar a implementação de jogos digitais para a plataforma DE1-SOC.

# Equipe de desenvolvimento
[Anesio Sousa](https://github.com/AnesioSousa)

[Caick Wendel](https://github.com/Caickk)

[Caike Dylon](https://github.com/Ckdylon)

# Descrição do problema
Foi implementado uma bibloioteca usando a liguagem assembly, desenvolvendo assim uma interface de conexão entre HPS e FPGA da DE1-SoC. sendo esse trabalho focado no desenvolvimento dessa conexão, unma vez que ao referente trabalho do Gabriel Sá Barreto Alves, desenvolvedor do processador grafico utilizado processador capaz de desenhar poligonos convexos, gerenciamento de sprites, porem o projeot original usa a NIOS II como processador principal, mas, na placa DE1-SoC adpta para ultilização do seu processador próprio HPS. O sistema é comandado pela placa DE1-SoC, sendo capaz de gerar imagem em um monitor via conexão VGA. O projetofoi desenvolvido acessando componetes do Processador Gráfico através de mapeamento de memória.

# Recursos ultilizados
O projeto foi desenvolvido na placa Intel® DE1-SoC, uma plataforma de grande versatilidade voltada para experimentações em organização de computadores e sistemas embarcados. Sua arquitetura é formada por dois componentes principais: o Hard Processor System (HPS) e o FPGA, ambos integrados no chip Cyclone® V SoC.

O HPS contém um processador ARM Cortex A9 dual-core, encarregado de executar o processamento de dados. Este sistema inclui também uma memória DDR3 e diversos dispositivos periféricos. O FPGA, por outro lado, hospeda dois processadores Intel Nios® II e possui várias portas periféricas, como memória, temporizadores, módulos de áudio, vídeo, PS/2, conversão analógica-digital e interfaces paralelas para interruptores e LEDs.

O sistema operacional utilizado é o Linux, e o desenvolvimento de software foi realizado com o uso das linguagens C e assembly, utilizando um editor de código que permite trabalhar com várias linguagens.

# Compilar e executar o game
Para rodar o game basta fazer o download do repositório e com um terminal aberto na pasta "app" executar o seguinte comando:
```
$ make
```
<h3> ⚠ Atenção! O sistema também depende da instalação prévia da biblioteca IntelFPGAUP!</h31>

# Mapeamento de Memoria 
Para possibilitar o acesso ao acelerômetro (ADXL345) presente na placa DE1-SOC, é necessário mapear a memória física para o espaço de endereçamento virtual. Esse processo envolve duas etapas principais.

## Acesso ao dev/men
O arquivo especial /dev/mem, em sistemas Linux, permite interação direta com o hardware, contornando as proteções do kernel. Para acessá-lo, são concedidas permissões de leitura e escrita, além de garantir sincronização de I/O.

## Mapeamento da Memória
Após a abertura do arquivo, o próximo passo é mapear os endereços do System Manager (SYSMGR) e do controlador I2C0. Durante esse processo, define-se o endereço base, que marca o ponto de partida na memória física, e o tamanho da área a ser mapeada (span), indicando a extensão da memória a ser acessada. O sistema operacional atribui um endereço virtual à área mapeada, permitindo leitura e escrita. O compartilhamento entre processos também é habilitado, possibilitando que diferentes partes do sistema alterem essa região de memória simultaneamente, garantindo uma comunicação eficiente entre componentes.

Com o mapeamento completo, deve-se configurar tanto o Pinmux quanto o controlador I2C0 para estabelecer a comunicação com o acelerômetro.

# Configuração do Pin Mux
Depois de mapear a base de endereços, os valores dos offsets de cada registrador são somados à base para acessar regiões específicas e realizar as configurações necessárias. A seguir estão os registradores essenciais para garantir o roteamento correto dos sinais I2C para o controlador I2C0:

GENERALIO7 (offset: 0x127): Este registrador é ajustado para o valor "1".
GENERALIO8 (offset: 0x128): Este registrador é ajustado para o valor "1".
I2C0USEFPGA (offset: 0x1C1): Este registrador é configurado para "0".

# Configuração do I2C0
Esses registradores são manipulados para inicializar e controlar a comunicação com o acelerômetro ADXL345. Os seguintes registradores são acessados, utilizando seus respectivos offsets:
<br>
**I2C_CON (0x0):** Define o modo de operação do I2C.
<br>
**I2C_TAR (0x4):** Define o endereço do dispositivo I2C de destino.
<br>
**I2C_DATA_CMD (0x10):** Envia e recebe dados.
<br>
**IC_FS_SCL_HCNT (0x1C) e IC_FS_SCL_LCNT (0x20):** Ajustam os tempos de sinal SCL.
<br>
**IC_CLR_INTR (0x40):** Limpa interrupções.
<br>
**IC_ENABLE (0x6C):** Habilita o módulo I2C.
<br>
**IC_TXFLR (0x74) e IC_RXFLR (0x78):** Indicam o nível dos buffers de transmissão/recepção.
<br>
**IC_ENABLE_STATUS (0x9C):** Indica o status de habilitação do controlador.
<br>

# Desenvolvimento
O projeto foi dividido em módulos para um melhor desenvolvimento. Sendo os modulo um com jogo Tetris, acelerômetro, biblioteca em assembly e algumas funções extras.

## Acelerometro
Nesse modulo a implementação de comunicação I2C para um acelerômetro (ADXL345), onde o código acessa o dispositivo através do /dev/mem e manipula os registradores diretamente. 

## GPU
### GPU_letters
Essa função é responsável pela criação das letras e números em uma submatriz 5x5, letras essas que seram usadas para a criação de textos e pontuações.
## GPU_lib




<br>
<br>
<br>
<br>
<br>
<br>

## Referências:
NAJIBGHADRI. Disponivel em: <https://github.com/najibghadri/Tetris200lines/tree/master>. Acesso em: 03/09/2024
<br>
VIMFULDANG. Disponivel em: <https://github.com/VimfulDang/ADXL345-Accelerometer-DE1-SOC>. Acesso em: 20/09/2024
