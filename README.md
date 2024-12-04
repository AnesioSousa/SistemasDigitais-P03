<div align="center"><img src="https://c4.wallpaperflare.com/wallpaper/459/983/696/pac-man-peach-retro-games-hd-wallpaper-preview.jpg"></div>

<br>

# 🟦🟩🟥🟨 PacMan 🟩🟨🟥🟦

Biblioteca com funções gráficas para o Processador Gráfico CoLenda, que permitem facilitar a implementação de jogos digitais para a plataforma DE1-SOC.

# Equipe de desenvolvimento
[Anesio Sousa](https://github.com/AnesioSousa)

[Caick Wendel](https://github.com/Caickk)

[Caike Dylon](https://github.com/Ckdylon)

# Sobre o projeto

PAC-MAN é um de arcade clássico que evoluiu significativamente desde sua apresentação à comunidade em 1980 pela Namco. As diretivas do game são simples, porém super engajadoras. O jogador controla o PAC-MAN, navegando ele através de um labirinto e enquanto anda o PAC-MAN vai consumindo pellets e a pontuação vai aumentando conforme a quantidade consumida por ele. O jogador deve sempre lembrar de evitar contato com os fantasmas coloridos.

# A mecânica do game 
**Objetivo:** O objetivo principal do jogo é consumir todas as pellets no labirinto enquando se evita ser pego pelos fantasmas. Comer as pellets maiores (chamadas de pellets do poder) faz com que o PAC-MAN temporariamente vire o jogo contra os fantastasmas, o tornando invulnerável e destruindo qualquer fastasma que o tocar.

**Nívels e labirintos:** O jogo consistem de múltiplos léveis, cada um com um design único de labirinto. A medida que os jogadores vão melhorando, a velocidade e o comportamento dos fantasmas vão se tornando mais desafiadores.

# Adaptações modernas
Na nossa versão do game, temos somente um labirinto, o objetivo de consumir todas as pellets ainda se mantém e o pacman ainda deve evitar contato com fantasmas. As diferenças começam na jogabilidade já que no jogo original os fantasmas perseguem automaticamente o PAC-MAN, e na nossa versão há somente um fantasma e ele é controlado por um segundo jogador utilizando o mouse.

Foi implementado uma biblioteca usando a liguagem assembly, desenvolvendo assim uma interface de conexão entre HPS e FPGA da DE1-SoC. Este projeto utiliza o trabalho de Gabriel Sá Barreto Alves, que desenvolveu um processador gráfico para desenhar polígonos convexos e gerenciar sprites. Diferentemente do projeto original, que usava o processador NIOS II, esta adaptação aproveita o processador HPS da DE1-SoC. A imagem é gerada em um monitor via VGA, acessando componentes do processador gráfico através do mapeamento de memória.

# Recursos ultilizados
O projeto foi desenvolvido na placa Intel® DE1-SoC, uma plataforma de grande versatilidade voltada para experimentações em organização de computadores e sistemas embarcados. Sua arquitetura é formada por dois componentes principais: o Hard Processor System (HPS) e o FPGA, ambos integrados no chip Cyclone® V SoC.

O HPS contém um processador ARM Cortex A9 dual-core, encarregado de executar o processamento de dados. Este sistema inclui também uma memória DDR3 e diversos dispositivos periféricos. O FPGA, por outro lado, hospeda dois processadores Intel Nios® II e possui várias portas periféricas, como memória, temporizadores, módulos de áudio, vídeo, PS/2, conversão analógica-digital e interfaces paralelas para interruptores e LEDs.

O sistema operacional utilizado é o Linux, e o desenvolvimento de software foi realizado com o uso das linguagens C e assembly, utilizando um editor de código que permite trabalhar com várias linguagens.

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
<br>
GENERALIO8 (offset: 0x128): Este registrador é ajustado para o valor "1".
<br>
I2C0USEFPGA (offset: 0x1C1): Este registrador é configurado para "0".
<br>

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
# Processador gráfico
O processador gráfico CoLenda é uma GPU desenvolvida em FPGA que gerencia polígonos convexos, como triângulos e quadrados, além de sprites em monitores VGA com resolução de 640x480 pixels. Ele opera em conjunto com um processador de propósito geral, como o Nios II, responsável pela lógica do jogo, enquanto o processador gráfico gerencia exclusivamente a renderização. Os sprites e outros elementos gráficos são armazenados em memórias dedicadas no hardware, como uma memória de sprites para imagens e uma memória de fundo para o cenário do jogo.
O processador gráfico realiza a renderização em tempo real. Ele gera os sinais necessários para monitores VGA, incluindo sincronização e dados de cor RGB, com uma taxa de atualização de 60 quadros por segundo. A comunicação entre o processador principal e o gráfico ocorre via FIFOs (First In, First Out), que armazenam comandos enviados pelo processador principal.
Além disso, o sistema possui um co-processador especializado em desenhar polígonos, como quadrados e triângulos, utilizando cálculos geométricos para determinar quais pixels pertencem à área de renderização.

# Desenvolvimento
O projeto foi dividido em módulos para um melhor desenvolvimento. Sendo os modulo um com jogo Pac-Man, acelerômetro, biblioteca em assembly e algumas funções extras.

## Acelerometro
Nesse modulo a implementação de comunicação I2C para um acelerômetro (ADXL345), onde o código acessa o dispositivo através do /dev/mem e manipula os registradores diretamente. 

## GPU
A manipulação da GPU foi dividida em dois blocos, que são eles:
### GPU_letters
Essa função é responsável pela criação das letras e números em uma submatriz 5x5, letras essas que seram usadas para a criação de textos e pontuações.
### GPU_lib
Nesse módulo é feita chamadas ao assembly para manipular diretamente a GPU, realizando operações de desenho de blocos, sprites e caracteres gráficos em um display. As funções permitem a renderização de elementos gráficos, como letras e números, bem como a configuração e limpeza do background.

## lib.s
Esse é o módulo com a biblioteca assembly. Contendo um conjunto de funções para realizar operações de mapeamento de memória, manipulando diretamente o espaço de memória e realizando instruções específicas de controle. 

### Funções principais:
- **mem_map:** Abre o arquivo de dispositivo /dev/mem e mapeia uma região de memória para acesso ao FPGA. Armazena o descritor de arquivo para operações futuras.
- **mem_unmap:** Desmapeia a região de memória previamente mapeada, liberando o espaço.
- **button:** Lê o estado de um botão conectado à FPGA através da memória mapeada.
- **clear_background:** Limpa o fundo, enviando instruções para a FPGA que redefinem o estado de memória relacionado ao fundo.
- **set_background_block:** Define um bloco de fundo específico em uma posição (linha e coluna) usando parâmetros recebidos, ajustando o estado de memória da FPGA.
- **set_sprite:** Define um sprite específico em uma posição, configurando detalhes como localização e opcode.
- **time_is_up:** Lê uma posição de memória específica para verificar se o tempo para uma determinada operação expirou.
- **reset_pulsecounter:** Reseta um contador de pulsos na FPGA, enviando um comando apropriado para o hardware.

# Compilar e executar o game
Para rodar o game basta fazer o download do repositório e com um terminal aberto na pasta "app" executar o seguinte comando:
```
$ make
```
<h3> ⚠ Atenção! O sistema também depende da instalação prévia da biblioteca IntelFPGAUP!</h31>



# Conclusão
Para a realização deste projeto, foi fundamental aplicar conhecimentos de interação entre hardware e software para desenvolvimento do produto final. A compreensão dos princípios da arquitetura da DE1-SoC, do mapeamento de memória, da programação em assembly e em C, bem como o uso da interface de comunicação I2C, da saída de vídeo VGA e de periféricos como botões, foram essenciais para integrar esses elementos. Ao final, o projeto atingiu os objetivos estabelecidos, proporcionando uma experiência de jogo completa e promovendo o aperfeiçoamento no uso de mapeamento de memória e na programação em assembly. Trabalhar com uma linguagem de baixo nível como o assembly, trouxe uma nova visão para o desenvolvimento de código, resultando em melhorias no desenvolvimento de software em geral.




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
<br>
G.L. Ronald J.Tocci, Neal S.Widmer, Sistemas Digitais Princípios e Aplicações. Pearson, 2019.

