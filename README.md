
# 🚀 **Atividade de Roteiro Nº 4 - Escalonamento de CPU** 🖥️

## 📝 **Descrição**
Esta atividade consiste em simular o **escalonamento de processos na CPU** utilizando o algoritmo **Round Robin**. O programa lê dados de um arquivo, cria processos e os gerencia com base em seus tempos de execução e prioridade. O **quantum** de CPU é calculado com base na mediana dos tempos de execução dos processos.

---

## 📄 **Objetivo**
O objetivo desta implementação é realizar o gerenciamento de processos de forma eficiente, calculando e distribuindo o quantum para cada processo. O algoritmo **Round Robin** é utilizado para garantir que todos os processos recebam uma fatia de tempo para execução de maneira justa.

---

## 🔧 **Tecnologias Utilizadas**

- **C++**: Linguagem utilizada para implementar o simulador de escalonamento.
- **Algoritmo Round Robin**: Utilizado para distribuir o tempo de CPU entre os processos.
- **Estrutura de Dados**: Lista duplamente encadeada para gerenciar a fila de processos.

---

## 🚦 **Fluxo de Execução**

1. **Leitura de Arquivo**: O programa lê dados de um arquivo (`processes.txt`), onde cada linha contém as informações de um processo, como nome, tempo de execução, prioridade, caminho e dono.
2. **Cálculo do Quantum**: A partir da mediana dos tempos de execução, o programa calcula o quantum de CPU para cada processo, ajustando-o com base na prioridade.
3. **Escalonamento**: O algoritmo **Round Robin** distribui o tempo de CPU entre os processos, garantindo que cada um seja executado por um tempo definido pelo quantum.
4. **Execução e Monitoramento**: A execução de cada processo é monitorada e os dados, como tempo restante e progresso, são exibidos na tela.

---

## 🔍 **Como Funciona a Distribuição do Quantum**

- **Cálculo da Mediana**: O primeiro passo é calcular a mediana dos tempos de execução de todos os processos. Isso ajuda a determinar um valor base para o quantum.
  
- **Ajuste com a Prioridade**: O quantum é ajustado com base na prioridade do processo. Processos de maior prioridade recebem um quantum proporcionalmente maior, garantindo que tenham mais tempo de CPU.

---

## ⚙️ **Como Usar**

### Passo 1: Preparar o arquivo de entrada

Crie um arquivo `processes.txt` com a seguinte estrutura:

```
Nome_do_Processo;Tempo_de_Execucao;Prioridade;Caminho;Dono
```

Exemplo:
```
processo1;10;3;/home/usuario;/home/usuario
processo2;15;1;/home/usuario;/home/usuario
processo3;20;2;/home/usuario;/home/usuario
```

### Passo 2: Compilar o Código

```bash
g++ -o escalonador main.cpp -std=c++11
```

### Passo 3: Executar o Programa

```bash
./escalonador
```

---

## 🧑‍💻 **Desenvolvedor**

- **Autor**: Marcos Vinicius Belo da Silva
- **Professor**: David Candeia Medeiros Maia

---

## 📚 **Referências**

- **Sistemas Operacionais** - Livro didático para fundamentação dos algoritmos de escalonamento.
- **Documentação de C++** - Referência para a utilização das bibliotecas padrão.

---

## 🏆 **Licença**

Este projeto está licenciado sob a [MIT License](LICENSE).
