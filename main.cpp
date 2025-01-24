/**
 * ============================================================================
 * Atividade de Roteiro Nº 4 - Disciplina de Sistemas Operacionais 
 * Tema: Atividade Prática de Escalonamento de CPU
 * 
 * Autor: Marcos Vinicius Belo da Silva
 * Professor: David Candeia Medeiros Maia
 * 
 * Descrição: 
 * Este código simula o escalonamento de CPU usando o algoritmo Round-Robin. 
 * A distribuição do quantum entre os processos é feita da seguinte maneira:
 * 
 * - Cada processo possui um valor de "quantum", que define o tempo máximo de CPU 
 *   que ele pode consumir antes de ser interrompido para que outro processo possa 
 *   ser executado.
 * - O quantum de cada processo é calculado dinamicamente com base no tempo médio 
 *   de execução dos processos (mediana dos tempos de execução) e a prioridade de 
 *   cada processo. A fórmula utilizada para calcular o quantum é:
 *     quantum = mediana * (1 + alfa * prioridade), onde alfa é um fator de ajuste.
 * - O cálculo do quantum considera que os processos de maior prioridade terão um 
 *   tempo de CPU mais curto, enquanto os de menor prioridade terão mais tempo.
 * 
 * ============================================================================
 */


#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <sstream>
#include <list>
#include <pthread.h>
#include <algorithm>
#include <atomic>

using namespace std;

/**
 * @brief Classe que representa um Processo.
 *
 * A classe Process contém informações sobre um processo em execução, como o nome,
 * tempo total, prioridade, tempo restante, e fornece métodos para atualizar seu 
 * estado e executar a simulação do processo.
 */
class Process {
public:
    /**
     * @brief Construtor padrão da classe Process.
     *
     * Inicializa os atributos do processo com valores padrão.
     */
    Process() : name(""), timeE(0), priority(0), path(""), owner(""), quantum(0), timeExecuted(0), timeRemaining(0) {}

    /**
     * @brief Construtor parametrizado da classe Process.
     *
     * @param name Nome do processo.
     * @param timeE Tempo total de execução do processo.
     * @param priority Prioridade do processo.
     * @param path Caminho do processo.
     * @param owner Dono do processo.
     */
    Process(string name, short int timeE, short int priority, string path, string owner) {
        this->name = name;
        this->timeE = timeE;
        this->priority = priority;
        this->path = path;
        this->owner = owner;
        this->enabled.store(true);
        this->quantum = 0;
        this->timeExecuted = 0;
        this->timeRemaining = timeE;
    }

    /**
     * @brief Método que imprime o estado atual do processo.
     *
     * Exibe informações como nome, tempo total, tempo restante, tempo na CPU e quantum.
     */
    void print() {
        system("clear");
        cout << "╔══════════════════════════════╗" << endl;
        cout << "║ \033[1;36m     Executando Processo\033[0m     ║" << endl;
        cout << "╚══════════════════════════════╝" << endl;
        cout << " Nome:        " << this->name << endl;
        cout << " Tempo Total: " << this->timeE << " segundos" << endl;
        cout << " Prioridade:  " << this->priority << endl;
        cout << " Caminho:     " << this->path << endl;
        cout << " Dono:        " << this->owner << endl;
        cout << "\033[1;33m-----------------------------------\033[0m" << endl;
        
        // Exibe o tempo restante e quantum
        cout << "\033[1;32mTempo na CPU:\033[0m " << this->timeExecuted << " segundos" << endl;
        cout << "\033[1;32mTempo Restante:\033[0m " << this->timeRemaining << " segundos" << endl;
        cout << "\033[1;34mQuantum:\033[0m " << this->quantum << endl;
    }

    /**
     * @brief Atualiza os valores de execução do processo.
     *
     * Verifica se o processo completou o quantum ou precisa atualizar o tempo restante e executado.
     */
    void updateValues(void) {
        if (this->timeExecuted >= this->quantum) {
            this->enabled.store(false);
            if(this->timeRemaining == 0)
                this->timeRemaining = this->timeE;
            this->timeExecuted = 0;
            return;
        }
        else if (this->timeRemaining > 0) {
            this->timeRemaining--;
        }
        this->timeExecuted++;
    }

    /**
     * @brief Executa o processo.
     *
     * Simula a execução do processo, exibindo o progresso e atualizando os valores até que o processo termine.
     */
    void execute() {
        this->enabled.store(true);
        while (this->enabled.load()) {
            this->print();
            int progress = (this->timeExecuted * 20) / this->timeE;  // Escala de 0 a 20
            cout << "\033[1;32mProgresso: [\033[0m";
            std::string block = "█";  // Define o bloco como string UTF-8
            if(this->timeExecuted < this->timeE)
                cout << string(this->timeExecuted, block[0]) << string(this->timeRemaining, '-');
            else
                cout << string(this->timeE, block[0]) << string(this->timeRemaining, '-');
            cout << "\033[1;32m]\033[0m" << endl;
            this->updateValues();
            sleep(1);
        }
        system("clear");
        cout << "\033[1;31mProcesso " << this->name << " terminado\033[0m" << endl;
        sleep(2);
    }

    /**
     * @brief Define o quantum do processo.
     *
     * @param value Valor do quantum a ser atribuído.
     */
    void setQuantum(short int value) { this->quantum = value; }

    /**
     * @brief Define o tempo restante do processo.
     *
     * @param value Tempo restante a ser atribuído.
     */
    void setTimeRemaining(short int value) { this->timeRemaining = value; }

    /**
     * @brief Obtém o nome do processo.
     *
     * @return Nome do processo.
     */
    string getName() const { return this->name; }

    /**
     * @brief Obtém o tempo total de execução do processo.
     *
     * @return Tempo total de execução.
     */
    short int getTimeE() const { return this->timeE; }

    /**
     * @brief Obtém a prioridade do processo.
     *
     * @return Prioridade do processo.
     */
    short int getPriority() const { return this->priority; }

    /**
     * @brief Obtém o caminho do processo.
     *
     * @return Caminho do processo.
     */
    string getPath() const { return this->path; }

    /**
     * @brief Obtém o dono do processo.
     *
     * @return Dono do processo.
     */
    string getOwner() const { return this->owner; }

    /**
     * @brief Calcula o quantum do processo baseado na mediana e no valor alpha.
     *
     * @param median Mediana dos tempos dos processos.
     * @param alpha Fator de ajuste baseado na prioridade.
     */
    void calcQuantum(int median, float alpha) {
        int quantum = static_cast<int>(median * (1 + alpha * this->priority));
        this->setQuantum(static_cast<short int>(quantum));
    }

    /**
     * @brief Destruidor da classe Process.
     */
    ~Process() {}

private:
    short int timeE;       /**< Tempo total de execução do processo */
    short int priority;    /**< Prioridade do processo */
    short int timeExecuted; /**< Tempo de execução do processo até o momento */
    short int timeRemaining; /**< Tempo restante para a conclusão */
    short int quantum;      /**< Quantum do processo */
    string name;            /**< Nome do processo */
    string path;            /**< Caminho do processo */
    string owner;           /**< Dono do processo */
    atomic<bool> enabled;   /**< Indica se o processo está habilitado para execução */
};

/**
 * @brief Classe que representa um nó em uma lista duplamente encadeada.
 *
 * Cada nó contém um ponteiro para um processo e referências para o próximo e anterior nó.
 */
class Node {
public:
    Node() : process(nullptr), next(nullptr), prev(nullptr) {}

    Node(Process* process) : process(process), next(nullptr), prev(nullptr) {}

    void setProcess(Process* process) { this->process = process; }
    void setNext(Node* node) { this->next = node; }
    void setPrev(Node* node) { this->prev = node; }

    Process* getProcess() const { return this->process; }
    Node* getNext() const { return this->next; }
    Node* getPrev() const { return this->prev; }

    ~Node() {}

private:
    Process* process; /**< Ponteiro para o processo associado ao nó */
    Node* next;       /**< Ponteiro para o próximo nó */
    Node* prev;       /**< Ponteiro para o nó anterior */
};

/**
 * @brief Classe que representa uma lista duplamente encadeada de processos.
 *
 * Esta lista mantém a ordem dos processos de acordo com a prioridade e permite operações de inserção e remoção.
 */
class DupEncList {
public:
    DupEncList() : head(nullptr), tail(nullptr), count(0) {}

    /**
     * @brief Adiciona um processo à lista.
     *
     * Calcula o quantum do processo e o adiciona à lista.
     *
     * @param process Ponteiro para o processo a ser adicionado.
     * @param median Mediana dos tempos dos processos.
     */
    void add(Process* process, int median) {
        Node* newNode = new Node(process);
        process->calcQuantum(median, 0.1);
        if (this->count == 0) {
            this->head = newNode;
            this->tail = newNode;
            newNode->setNext(newNode);
            newNode->setPrev(newNode);
        } else {
            newNode->setPrev(this->tail);
            newNode->setNext(this->head);
            this->tail->setNext(newNode);
            this->head->setPrev(newNode);
            this->tail = newNode;
        }
        this->count++;
    }

    /**
     * @brief Remove o último processo da lista.
     */
    void pop() {
        if (this->count == 0) return;

        if (this->count == 1) {
            delete this->tail;
            this->head = nullptr;
            this->tail = nullptr;
        } else {
            Node* temp = this->tail;
            this->head->setPrev(this->tail->getPrev());
            this->tail->getPrev()->setNext(this->head);
            this->tail = this->tail->getPrev();
            delete temp;
        }
        this->count--;
    }

    /**
     * @brief Encontra um nó contendo um determinado processo.
     *
     * @param process Ponteiro para o processo a ser encontrado.
     * @return Ponteiro para o nó contendo o processo, ou nullptr se não encontrado.
     */
    Node* find(Process* process) {
        Node* current = this->head;
        for (int i = 0; i < this->count; i++) {
            if (current->getProcess() == process)
                break;
            current = current->getNext();
        }
        return current;
    }

    /**
     * @brief Remove um processo da lista.
     *
     * @param process Ponteiro para o processo a ser removido.
     */
    void remove(Process* process) {
        Node* node = find(process);
        if (node != nullptr) {
            node->getPrev()->setNext(node->getNext());
            node->getNext()->setPrev(node->getPrev());
            delete node;
            this->count--;
        }
    }

    /**
     * @brief Obtém o número de processos na lista.
     *
     * @return Número de processos na lista.
     */
    const int getCount() const { return this->count; }

    /**
     * @brief Executa todos os processos da lista.
     *
     * Este método chama a execução de cada processo na lista.
     */
    void executeAll() const {
        Node *current = this->head;
        while(true) {
            if(current->getProcess() != nullptr){
                current->getProcess()->execute();
            }
            current = current->getNext();
        }
    }

    /**
     * @brief Destruidor da classe DupEncList.
     *
     * Libera toda a memória alocada pelos nós da lista.
     */
    ~DupEncList() {
        if (this->count == 0) return;

        Node* current = this->head;
        Node* nextNode = nullptr;

        for (int i = 0; i < this->count; i++) {
            nextNode = current->getNext();
            delete current;
            current = nextNode;
        }
    }

private:
    Node* head; /**< Cabeça da lista */
    Node* tail; /**< Cauda da lista */
    int count;  /**< Número de processos na lista */
};

/**
 * @brief Calcula a mediana dos tempos de execução dos processos.
 *
 * @param ProcessArray Vetor de ponteiros para os processos.
 * @return A mediana dos tempos de execução.
 */
int calcMedian(Process* ProcessArray[7]) {
    vector<int> timeArray;

    for (int i = 0; i < 7; i++) {
        timeArray.push_back(ProcessArray[i]->getTimeE());
    }

    sort(timeArray.begin(), timeArray.end());
    return timeArray[timeArray.size() / 2];
}

/**
 * @brief Organiza os processos em ordem de prioridade e os adiciona à lista de execução.
 *
 * @param ProcessArray Vetor de ponteiros para os processos.
 * @return Ponteiro para a lista de execução ordenada.
 */
DupEncList* RoundRobin(Process* ProcessArray[7]) {
    int median = calcMedian(ProcessArray);

    DupEncList* listExecution = new DupEncList();
    for (int priority = 6; priority > 0; priority--) {
        for (int pos = 0; pos <= 6; pos++) {
            if (ProcessArray[pos]->getPriority() == priority) {
                listExecution->add(ProcessArray[pos], median);
            }
        }
    }
    return listExecution;
}

/**
 * @brief Lê dados de um arquivo e cria processos a partir deles.
 *
 * @param fileName Nome do arquivo a ser lido.
 * @param ProcessArray Vetor para armazenar os processos lidos.
 * @return True se a leitura foi bem-sucedida, false caso contrário.
 */
bool readfile(string fileName, Process* ProcessArray[7]) {
    try {
        int count = 0;
        ifstream inputFile(fileName);

        if (!inputFile) {
            cerr << "Erro ao abrir o arquivo!" << endl;
            return false;
        }

        string line;
        while (getline(inputFile, line) && count < 7) {
            if (line == "-------- fim do arquivo --------")
                break;
            if (line == "------- início do arquivo -------" || line.empty())
                continue;

            if (line[0] == ';') {
                cerr << "Linha ignorada por formato inválido: " << line << endl;
                continue;
            }

            vector<string> processData;
            stringstream ss(line);
            string token;

            while (getline(ss, token, ';')) {
                token.erase(0, token.find_first_not_of(" \t"));
                token.erase(token.find_last_not_of(" \t") + 1);
                processData.push_back(token);
            }

            if (processData.size() != 5) {
                cerr << "Linha com número de campos inválido: " << line << endl;
                continue;
            }

            string name = processData[0];
            short int timeE = stoi(processData[1]);
            short int priority = stoi(processData[2]);
            string path = processData[3];
            string owner = processData[4];

            ProcessArray[count] = new Process(name, timeE, priority, path, owner);
            count++;
        }

        inputFile.close();

        if (count < 7) {
            cerr << "Número insuficiente de processos no arquivo. Esperado: 7, Lido: " << count << endl;
            return false;
        }

        return true;
    } catch (const std::exception& e) {
        cerr << "Erro durante a leitura do arquivo: " << e.what() << '\n';
        return false;
    }
}

/**
 * @brief Função principal que inicializa e executa os processos.
 *
 * Lê o arquivo de entrada, cria a lista de execução e executa os processos no esquema Round-Robin.
 *
 * @return Código de status (0 para sucesso, -1 para erro).
 */
int main(void) {
    string fileName = "processes.txt";
    Process* ProcessArray[7];
    if (!readfile(fileName, ProcessArray)) {
        return -1;
    }

    DupEncList* executeList = RoundRobin(ProcessArray);

    try {
        executeList->executeAll();
    } catch (const std::exception& e) {
        delete executeList;
        cerr << e.what() << '\n';
    }

    delete executeList;
    return 0;
}
