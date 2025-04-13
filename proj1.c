#include "proj1.h"

/*Projeto de Introdução aos Algoritmos e estruturas de Dados
Henrique Lhano
Número: 109281
henrique.lhano@tecnico.ulisboa.pt*/


/* Funções para parsing do input */

/* Le espaços. Devolve 0 se chegou ao final de linha ou 1 caso contrário. */
int leEspacos() {
    int c;
    while ((c = getchar()) == ' ' || c == '\t');
    if (c == '\n') 
      return 0;
    ungetc(c, stdin);
    return 1;
}

/* Le um nome para a string que recebe como parametro. */  
void leNome(char s[]) {
    int i = 0, c;
    s[0] = getchar();
    if (s[0] != '"') {
      i = 1;
      while ((c = getchar()) != ' ' && c != '\t' && c != '\n')
        s[i++] = c;
      ungetc(c, stdin);
    }
    else {
      while((c = getchar()) != '"')
        s[i++] = c;
    }
    s[i++] = '\0';
}
  
/* Le todo o texto até ao final de linha. */
void leAteFinalLinha(char s[]) {
    char c;
    int i = 0;
    
    if (s == NULL) { // Caso especial: descartar a linha sem armazenar
        while ((c = getchar()) != '\n' && c != EOF);
        return;
    }
    
    while ((c = getchar()) != '\n' && c != EOF) {
        s[i++] = c;
    }
    s[i] = '\0';
}
  
/* Inicializar a data do sistema a 01-01-2025 */
void inicializarData(){
    _dataActual.dia = 1;
    _dataActual.mes = 1;
    _dataActual.ano = 2025;
}

/*Ver se o nome do lote existe no sistema*/
int loteExiste(char *s) {
  for (int i = 0; i < _numLotes; i++) {
    if (strcmp(s, _lotes[i]->lote) == 0)  
      return i;
  }
  return NAO_ENCONTRADO;
}

/*Ver se o nome da vacina existe no sistema*/
int vacinaExiste(char *s) {
  for (int i = 0; i < _numLotes; i++) {
    if (strcmp(s, _lotes[i]->nome) == 0) 
      return i;
  }
  return NAO_ENCONTRADO;
}

/* Converte a data que lê de input para uma estrutura do tipo Data*/
Data* converteTextoData(char* data) {
  Data* dh = (Data*)malloc(sizeof(Data));
  sscanf(data, "%d-%d-%d", &(dh->dia), &(dh->mes), &(dh->ano));
  return dh;
}

/*Conjunto de letras maiúsculas inválidas*/
int letraInvalida(char c) {
  return (c > 'F' && c <= 'Z');
}

/*Garantir que o nome do lote a registar é válido*/
int validaLote(char* lote){
    if (strlen(lote) >= MAX_CHAR_LOTE){
      return -1;
    }
    
    for (size_t i = 0; i < strlen(lote); i++){
        if (letraInvalida(lote[i])){
            return -1;
        }      
        if (islower(lote[i])){
          return -1;
        }
        
        
    }
    return 1;
}

/*Garantir que o nome da vacina a registar é válido*/
int validaVacina(char* vacina){
  for (int i = 0; i < MAX_CHAR_VACINA; i++){
      if (strlen(vacina) > 51){
        return -1;
      }      
  }
  return 1;
}

/* Número de dias que o mês de argumento tem*/
int diasNoMes(int mes) {
    if (mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 || mes == 10 || mes == 12) {
        return 31;
    }
    if (mes == 2){
        return 28;
    }
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11){
        return 30;
    }
    return 0;
}

/*Garantir que uma estrutura do tipo Data é válida*/
int dataValida(Data* dataaverificar) {
    if (dataaverificar->mes < 1 || dataaverificar->mes > 12) {
        return 0; // Mês inválido
    }
    if (dataaverificar->dia < 1 || dataaverificar->dia > diasNoMes(dataaverificar->mes)) {
        return 0; // Dia inválido
    }
    if (dataaverificar->mes == 2 && ((dataaverificar->ano % 4 == 0 && dataaverificar->ano % 100 != 0) || dataaverificar->ano % 400 == 0)) {
        if (dataaverificar->dia > 29) {
            return 0; // Dia inválido em ano bissexto
        }
    }
    if (dataaverificar->ano < 2025){
      return 0;
    }
    return 1; // Data válida
}

/*Atualiza a data em que o sistema se encontra*/
void actualizaDataActual(Data* d) {
  _dataActual = (*d);
}


/*Função responsável por tratar do comando c*/
void insereLote() {
    char loteTemp[200000] = {0};
    char data[MAX_CHAR_DATA], vacinaTemp[200000];
    int numeroDoses;
    Data* dataEntrada = NULL;

    leEspacos();
    leNome(loteTemp);

    if (validaLote(loteTemp) == -1) {
        printf("invalid batch\n");
        leAteFinalLinha(loteTemp);
        return;
    }

    int indexLote = loteExiste(loteTemp);

    if (indexLote != NAO_ENCONTRADO) {
        printf("duplicate batch number\n");
        leAteFinalLinha(loteTemp);
        return;
    }

    leEspacos();
    leNome(data);
    
    leEspacos();
    scanf("%d", &numeroDoses);

    if (numeroDoses <= 0) {
        printf("invalid quantity\n");
        return;
    }

    leEspacos();
    leNome(vacinaTemp);

    if (validaVacina(vacinaTemp) == -1) {
        printf("invalid name\n");
        return;
    }

    if (_numLotes >= MAX_LOTES) {
        printf("too many vaccines\n");
        return;
    }
    dataEntrada = converteTextoData(data);
    if (dataValida(dataEntrada)) {
        // Para não permitir datas anteriores à data atual
        if (dataEntrada->ano > _dataActual.ano || 
            (dataEntrada->ano == _dataActual.ano && dataEntrada->mes > _dataActual.mes) || 
            (dataEntrada->ano == _dataActual.ano && dataEntrada->mes == _dataActual.mes && dataEntrada->dia >= _dataActual.dia)) {   
            if (indexLote == NAO_ENCONTRADO) {
                _lotes[_numLotes] = (LoteVacina*)malloc(sizeof(LoteVacina));
                _lotes[_numLotes]->lote = strdup(loteTemp); // Aloca memória e copia o lote
                _lotes[_numLotes]->nome = strdup(vacinaTemp); // Aloca memória e copia o nome da vacina
                _lotes[_numLotes]->registo = _registos[_numRegistos];
                _lotes[_numLotes]->doses_aplicadas = 0;
                _lotes[_numLotes]->doses_disponiveis = numeroDoses;
                _lotes[_numLotes]->registo = NULL;
                _numLotes++;
    
                _registos[_numRegistos] = (RegistoLote*)malloc(sizeof(RegistoLote));
                _lotes[_numLotes - 1]->registo = _registos[_numRegistos];
                _registos[_numRegistos]->lote = _lotes[_numLotes - 1];
                _registos[_numRegistos]->entrada = dataEntrada;
                _numRegistos++;
                printf("%s\n", _lotes[_numLotes - 1]->lote);
            }
        } else {
            printf("invalid date\n");
        }
    } else {
        printf("invalid date\n");
    }
}

/* Função auxiliar para filtrar os lotes disponíveis*/
int filtraLotes(LoteVacina* lotesOrdenados[], int maxLotes) {
    int count = 0;
    for (int i = 0; i < _numLotes; i++) {
        if (_lotes[i] != NULL && count < maxLotes) {
            lotesOrdenados[count++] = _lotes[i];
        }
    }
    return count;
}

/*Função auxiliar para ordenar os lotes por ordem cronológica e alfabética*/
void sortLotes(LoteVacina* lotes[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            Data* dataI = lotes[i]->registo->entrada;
            Data* dataJ = lotes[j]->registo->entrada;
            if (dataJ->ano < dataI->ano || 
                (dataJ->ano == dataI->ano && dataJ->mes < dataI->mes) || 
                (dataJ->ano == dataI->ano && dataJ->mes == dataI->mes && dataJ->dia < dataI->dia) || 
                (dataJ->ano == dataI->ano && dataJ->mes == dataI->mes && dataJ->dia == dataI->dia && strcmp(lotes[j]->lote, lotes[i]->lote) < 0)) {
                LoteVacina* temp = lotes[i];
                lotes[i] = lotes[j];
                lotes[j] = temp;
            }
        }
    }
}

/*Função responsável por tratar do comando l*/
void listarLotes(char* args) {
    LoteVacina* lotesOrdenados[MAX_LOTES];
    int numLotesOrdenados = filtraLotes(lotesOrdenados, MAX_LOTES);
    sortLotes(lotesOrdenados, numLotesOrdenados);

    if (args != NULL) {
        char* token = strtok(args, " ");
        while (token) {
            int found = 0;
            for (int i = 0; i < numLotesOrdenados; i++) {
                if (strcmp(lotesOrdenados[i]->nome, token) == 0) {
                    Data* data = lotesOrdenados[i]->registo->entrada;
                    printf("%s %s %02d-%02d-%04d %d %d\n",
                           lotesOrdenados[i]->nome,
                           lotesOrdenados[i]->lote,
                           data->dia, data->mes, data->ano,
                           lotesOrdenados[i]->doses_disponiveis,
                           lotesOrdenados[i]->doses_aplicadas);
                    found = 1;
                }
            }
            if (!found) {
                printf("%s: no such vaccine\n", token);
            }
            token = strtok(NULL, " ");
        }
    } else {
        for (int i = 0; i < numLotesOrdenados; i++) {
            Data* data = lotesOrdenados[i]->registo->entrada;
            printf("%s %s %02d-%02d-%04d %d %d\n",
                   lotesOrdenados[i]->nome,
                   lotesOrdenados[i]->lote,
                   data->dia, data->mes, data->ano,
                   lotesOrdenados[i]->doses_disponiveis,
                   lotesOrdenados[i]->doses_aplicadas);
        }
    }
}

/*Função auxiliar para verificar se o utente já foi vacinado hoje*/
int utenteJaVacinadoHoje(const char* nomeUtente, const char* nomeVacina) {
    for (int i = 0; i < _numUtentes; i++) {
        if (strcmp(_utentes[i].nomeUtente, nomeUtente) == 0 &&
            strcmp(_utentes[i].nomeVacina, nomeVacina) == 0 &&
            _utentes[i].dataVacinacao.dia == _dataActual.dia &&
            _utentes[i].dataVacinacao.mes == _dataActual.mes &&
            _utentes[i].dataVacinacao.ano == _dataActual.ano) {
            return 1; // Já vacinado
        }
    }
    return 0; // Não vacinado
}

/*Função responsável por tratar do comando a*/
void aplicarDose() {
    char nomeUtenteTemp[200000], nomeVacinaTemp[200000];
    leEspacos();
    leNome(nomeUtenteTemp);
    leEspacos();
    leNome(nomeVacinaTemp);
    
    if (utenteJaVacinadoHoje(nomeUtenteTemp, nomeVacinaTemp)) {
        printf("already vaccinated\n");
        return;
    }    
    
    int indexLote = -1;
    Data dataMaisAntiga = { .ano = 9999, .mes = 12, .dia = 31 }; // Inicializa com uma data futura
    
    for (int i = 0; i < _numLotes; i++) {
        if (strcmp(_lotes[i]->nome, nomeVacinaTemp) == 0 && _lotes[i]->doses_disponiveis > 0) {
            Data loteData = *_lotes[i]->registo->entrada;
            if (loteData.ano < dataMaisAntiga.ano ||
                (loteData.ano == dataMaisAntiga.ano && loteData.mes < dataMaisAntiga.mes) ||
                (loteData.ano == dataMaisAntiga.ano && loteData.mes == dataMaisAntiga.mes && loteData.dia < dataMaisAntiga.dia)) {
                indexLote = i;
                dataMaisAntiga = loteData;
            }
        }
    }
    
    if (indexLote == -1) {
        printf("no stock\n");
        return;
    }
    
    // Aplica a dose
    _lotes[indexLote]->doses_disponiveis--;
    _lotes[indexLote]->doses_aplicadas++;
    
    // Regista a vacinação do utente
    _utentes[_numUtentes].nomeUtente = strdup(nomeUtenteTemp);
    _utentes[_numUtentes].nomeVacina = strdup(nomeVacinaTemp);
    _utentes[_numUtentes].nomeLote = strdup(_lotes[indexLote]->lote);
    _utentes[_numUtentes].dataVacinacao = _dataActual;
    _utentes[_numUtentes].numeroVacinacoes++;
    _numUtentes++;
    
    printf("%s\n", _lotes[indexLote]->lote);
}

/*Função responsável por tratar do comando r*/
void retiraDisponibilidade() {
    char lote[200000];
    leEspacos();
    leNome(lote);
    int indexLote = loteExiste(lote);

    if (indexLote == NAO_ENCONTRADO) {  
        printf("%s: no such batch\n", lote);
        leAteFinalLinha(lote);
        return;
    }

    printf("%d\n", _lotes[indexLote]->doses_aplicadas);

    if (_lotes[indexLote]->doses_aplicadas == 0) {
        // Se não houve inoculações, remover completamente
        for (int i = 0; i < _numRegistos; i++) {
            if (_registos[i]->lote == _lotes[indexLote]) {
                free(_registos[i]->entrada);
                free(_registos[i]);
                for (int j = i; j < _numRegistos - 1; j++) {
                    _registos[j] = _registos[j + 1];
                }
                _numRegistos--;
                break;
            }
        }

        free(_lotes[indexLote]->nome);
        free(_lotes[indexLote]->lote);
        free(_lotes[indexLote]);

        for (int i = indexLote; i < _numLotes - 1; i++) {
            _lotes[i] = _lotes[i + 1];
        }
        _numLotes--;
    } else {
        // Se houver inoculações, manter o lote com doses_aplicadas
        _lotes[indexLote]->doses_disponiveis = 0;
    }
}

/*Função responsável por tratar do comando d*/
void apagarAplicacoes() {
    char nomeUtenteTemp[200000];
    leEspacos();
    leNome(nomeUtenteTemp);
    // Caso seja com o nome do utente e possivelmente uma data
    if (leEspacos()) {
        char dataTemp[MAX_CHAR_DATA];
        leNome(dataTemp);

        // Converte a data fornecida para a estrutura Data
        Data* dataApagar = converteTextoData(dataTemp);
        if (!dataValida(dataApagar)) {
            printf("invalid date\n");
            free(dataApagar);
            return;
        }

        int totalApagados = 0;

        for (int i = 0; i < _numUtentes; ) {
            if (strcmp(_utentes[i].nomeUtente, nomeUtenteTemp) == 0 &&
                _utentes[i].dataVacinacao.dia == dataApagar->dia &&
                _utentes[i].dataVacinacao.mes == dataApagar->mes &&
                _utentes[i].dataVacinacao.ano == dataApagar->ano) {
                // Liberta memória associada ao registo do utente
                free(_utentes[i].nomeUtente);
                free(_utentes[i].nomeVacina);
                free(_utentes[i].nomeLote);

                // Remove o registo deslocando os elementos seguintes
                for (int j = i; j < _numUtentes - 1; j++) {
                    _utentes[j] = _utentes[j + 1];
                }
                _numUtentes--; // Decrementa o número total de utentes
                totalApagados++;
            } else {
                i++; // Avança apenas se não removeu o registo
            }
        }

        free(dataApagar); // Liberta a memória alocada para a data
        printf("%d\n", totalApagados); // Imprime o número total de registos apagados
    } else {
        // Caso seja apenas com o nome do utente
        int totalApagados = 0;

        for (int i = 0; i < _numUtentes; ) {
            if (strcmp(_utentes[i].nomeUtente, nomeUtenteTemp) == 0) {
                // Liberta memória associada ao registo do utente
                free(_utentes[i].nomeUtente);
                free(_utentes[i].nomeVacina);
                free(_utentes[i].nomeLote);

                // Remove o registo deslocando os elementos seguintes
                for (int j = i; j < _numUtentes - 1; j++) {
                    _utentes[j] = _utentes[j + 1];
                }
                _numUtentes--; // Decrementa o número total de utentes
                totalApagados++;
            } else {
                i++; // Avança apenas se não removeu o registo
            }
        }

        printf("%d\n", totalApagados); // Imprime o número total de registos apagados
    }
}

/*Função responsável por tratar do comando u*/
void listarAplicacoes() {
    if (leEspacos()) {
        char nomeUtente[200000]; // Supondo um tamanho máximo para o nome
        leNome(nomeUtente); // Lê o nome do utente

        int encontrou = 0;

        // Percorre todos os registos de utentes em busca do nome fornecido
        for (int i = 0; i < _numUtentes; i++) {
            if (strcmp(_utentes[i].nomeUtente, nomeUtente) == 0) {
                printf("%s %s %02d-%02d-%04d\n",
                       _utentes[i].nomeUtente,
                       _utentes[i].nomeLote,
                       _utentes[i].dataVacinacao.dia,
                       _utentes[i].dataVacinacao.mes,
                       _utentes[i].dataVacinacao.ano);
                encontrou = 1;
            }
        }

        
        if (!encontrou) {
            printf("%s: no such user\n", nomeUtente);
        }

    } else {

        // Ordena os registos por ordem cronológica
        for (int i = 0; i < _numUtentes - 1; i++) {
            for (int j = i + 1; j < _numUtentes; j++) {
                if ((_utentes[i].dataVacinacao.ano > _utentes[j].dataVacinacao.ano) ||
                    (_utentes[i].dataVacinacao.ano == _utentes[j].dataVacinacao.ano &&
                     _utentes[i].dataVacinacao.mes > _utentes[j].dataVacinacao.mes) ||
                    (_utentes[i].dataVacinacao.ano == _utentes[j].dataVacinacao.ano &&
                     _utentes[i].dataVacinacao.mes == _utentes[j].dataVacinacao.mes &&
                     _utentes[i].dataVacinacao.dia > _utentes[j].dataVacinacao.dia)) {
                    
                    // Troca os registos para ordenar corretamente
                    Utente temp = _utentes[i];
                    _utentes[i] = _utentes[j];
                    _utentes[j] = temp;
                }
            }
        }

        // Imprime os registos das inoculações ordenados
        for (int i = 0; i < _numUtentes; i++) {
            printf("%s %s %02d-%02d-%04d\n",
                   _utentes[i].nomeUtente,
                   _utentes[i].nomeLote,
                   _utentes[i].dataVacinacao.dia,
                   _utentes[i].dataVacinacao.mes,
                   _utentes[i].dataVacinacao.ano);
        }
    }
}

/*Função responsável por tratar do comando t*/
void avancaTempo(){
  char data[MAX_CHAR_DATA];
  if (leEspacos()){
    leNome(data);
    Data *novaData = converteTextoData(data);
    if (dataValida(novaData)) {
      // Verifica se a nova data é posterior ou igual à data atual
      if (novaData->ano > _dataActual.ano ||
          (novaData->ano == _dataActual.ano && novaData->mes > _dataActual.mes) ||
          (novaData->ano == _dataActual.ano && novaData->mes == _dataActual.mes && novaData->dia >= _dataActual.dia)) {
          actualizaDataActual(novaData);
          printf("%02d-%02d-%02d\n", novaData->dia, novaData->mes,novaData->ano);
          free(novaData);
      } else {
          printf("invalid date\n"); // Data anterior à atual, inválida no contexto do sistema
          free(novaData);
      }
    } else {
        printf("invalid date\n"); 
        free(novaData);
    }
  }else{
    printf("%02d-%02d-%02d\n", _dataActual.dia, _dataActual.mes, _dataActual.ano);
    return;
  }
} 


/*Função responsável por libertar a memória associada aos lotes*/
void libertaLotes() {
    for (int i = 0; i < _numLotes; i++) {
      free(_lotes[i]->nome); // Liberta a memória alocada para o nome da vacina
      free(_lotes[i]->lote); // Liberta a memória alocada para o nome do lote
      free(_lotes[i]);       // Liberta a memória alocada para a estrutura do lote
    }
    _numLotes = 0; // Mete a contagem a 0
}

/*Função responsável por libertar a memória associada aos registos*/
void libertaRegistos() {
    for (int i = 0; i < _numRegistos; i++) {
        if (_registos[i]) { 
            free(_registos[i]->entrada); // Liberta primeiro a data de entrada
            free(_registos[i]);          // Depois liberta a estrutura principal
        }
    }
    _numRegistos = 0; // Mete a contagem a 0 
}

/*Função responsável por libertar a memória associada aos utentes*/
void libertaUtentes(){
    for (int i = 0; i < _numUtentes; i++){
        free(_utentes[i].nomeUtente);
        free(_utentes[i].nomeVacina);
        free(_utentes[i].nomeLote);
    }
    _numUtentes = 0; // Mete a contagem a 0
}

/* As funções em diante também são responsáveis pelos comandos, 
mas estas têm a mensagem de erro em português. 
São chamadas no switch-case da main caso o argumento seja pt. */ 

void insereLotePT() {
    char loteTemp[200000] = {0}, data[MAX_CHAR_DATA], vacinaTemp[200000] = {0};
    int numeroDoses;
    Data* dataEntrada = NULL;

    leEspacos();
    leNome(loteTemp);

    if (validaLote(loteTemp) == -1) {
        printf("lote inválido\n");
        leAteFinalLinha(loteTemp);
        return;
    }

    int indexLote = loteExiste(loteTemp);

    if (indexLote != NAO_ENCONTRADO) {
        printf("número de lote duplicado\n");
        leAteFinalLinha(loteTemp);
        return;
    }

    leEspacos();
    leNome(data);

    leEspacos();
    scanf("%d", &numeroDoses);

    if (numeroDoses <= 0) {
        printf("quantidade inválida\n");
        return;
    }

    leEspacos();
    leNome(vacinaTemp);

    if (validaVacina(vacinaTemp) == -1) {
        printf("nome inválido\n");
        return;
    }

    if (_numLotes >= MAX_LOTES) {
        printf("demasiadas vacinas\n");
        return;
    }

    dataEntrada = converteTextoData(data);
    if (dataValida(dataEntrada)) {
        // Para não permitir datas anteriores à data atual
        if (dataEntrada->ano > _dataActual.ano || 
            (dataEntrada->ano == _dataActual.ano && dataEntrada->mes > _dataActual.mes) || 
            (dataEntrada->ano == _dataActual.ano && dataEntrada->mes == _dataActual.mes && dataEntrada->dia >= _dataActual.dia)) {   
            if (indexLote == NAO_ENCONTRADO) {
                _lotes[_numLotes] = (LoteVacina*)malloc(sizeof(LoteVacina));
                _lotes[_numLotes]->lote = strdup(loteTemp); // Aloca memória e copia o lote
                _lotes[_numLotes]->nome = strdup(vacinaTemp); // Aloca memória e copia o nome da vacina
                _lotes[_numLotes]->registo = _registos[_numRegistos];
                _lotes[_numLotes]->doses_aplicadas = 0;
                _lotes[_numLotes]->doses_disponiveis = numeroDoses;
                _lotes[_numLotes]->registo = NULL;
                _numLotes++;
    
                _registos[_numRegistos] = (RegistoLote*)malloc(sizeof(RegistoLote));
                _lotes[_numLotes - 1]->registo = _registos[_numRegistos];
                _registos[_numRegistos]->lote = _lotes[_numLotes - 1];
                _registos[_numRegistos]->entrada = dataEntrada;
                _numRegistos++;
                printf("%s\n", _lotes[_numLotes - 1]->lote);
            }
        } else {
            printf("data inválida\n");
        }
    } else {
        printf("data inválida\n");
    }
}

void listarLotesPT(char* args) {
    LoteVacina* lotesOrdenados[MAX_LOTES];
    int numLotesOrdenados = filtraLotes(lotesOrdenados, MAX_LOTES);
    sortLotes(lotesOrdenados, numLotesOrdenados);

    if (args != NULL) {
        char* token = strtok(args, " ");
        while (token) {
            int found = 0;
            for (int i = 0; i < numLotesOrdenados; i++) {
                if (strcmp(lotesOrdenados[i]->nome, token) == 0) {
                    Data* data = lotesOrdenados[i]->registo->entrada;
                    printf("%s %s %02d-%02d-%04d %d %d\n",
                           lotesOrdenados[i]->nome,
                           lotesOrdenados[i]->lote,
                           data->dia, data->mes, data->ano,
                           lotesOrdenados[i]->doses_disponiveis,
                           lotesOrdenados[i]->doses_aplicadas);
                    found = 1;
                }
            }
            if (!found) {
                printf("%s: vacina inexistente\n", token);
            }
            token = strtok(NULL, " ");
        }
    } else {
        for (int i = 0; i < numLotesOrdenados; i++) {
            Data* data = lotesOrdenados[i]->registo->entrada;
            printf("%s %s %02d-%02d-%04d %d %d\n",
                   lotesOrdenados[i]->nome,
                   lotesOrdenados[i]->lote,
                   data->dia, data->mes, data->ano,
                   lotesOrdenados[i]->doses_disponiveis,
                   lotesOrdenados[i]->doses_aplicadas);
        }
    }
}

void aplicarDosePT() {
    char nomeUtenteTemp[200000], nomeVacinaTemp[200000];
    leEspacos();
    leNome(nomeUtenteTemp);
    leEspacos();
    leNome(nomeVacinaTemp);
    
    // Verifica se o utente já foi vacinado hoje com a mesma vacina
    if (utenteJaVacinadoHoje(nomeUtenteTemp, nomeVacinaTemp)) {
        printf("já vacinado\n");
        return;
    } 
    
    int indexLote = -1;
    Data dataMaisAntiga = { .ano = 9999, .mes = 12, .dia = 31 }; // Inicializa com uma data futura
    
    for (int i = 0; i < _numLotes; i++) {
        if (strcmp(_lotes[i]->nome, nomeVacinaTemp) == 0 && _lotes[i]->doses_disponiveis > 0) {
            Data loteData = *_lotes[i]->registo->entrada;
            if (loteData.ano < dataMaisAntiga.ano ||
                (loteData.ano == dataMaisAntiga.ano && loteData.mes < dataMaisAntiga.mes) ||
                (loteData.ano == dataMaisAntiga.ano && loteData.mes == dataMaisAntiga.mes && loteData.dia < dataMaisAntiga.dia)) {
                indexLote = i;
                dataMaisAntiga = loteData;
            }
        }
    }
    
    if (indexLote == -1) {
        printf("esgotado\n");
        return;
    }
    
    // Aplica a dose
    _lotes[indexLote]->doses_disponiveis--;
    _lotes[indexLote]->doses_aplicadas++;
    
    // Regista a vacinação do utente
    _utentes[_numUtentes].nomeUtente = strdup(nomeUtenteTemp);
    _utentes[_numUtentes].nomeVacina = strdup(nomeVacinaTemp);
    _utentes[_numUtentes].nomeLote = strdup(_lotes[indexLote]->lote);
    _utentes[_numUtentes].dataVacinacao = _dataActual;
    _utentes[_numUtentes].numeroVacinacoes++;
    _numUtentes++;
    
    printf("%s\n", _lotes[indexLote]->lote);
}

void retiraDisponibilidadePT() {
    char lote[200000];
    leEspacos();
    leNome(lote);
    int indexLote = loteExiste(lote);

    if (indexLote == NAO_ENCONTRADO) {  // Se não existe, erro
        printf("%s: lote inexistente\n", lote);
        leAteFinalLinha(lote);
        return;
    }

    printf("%d\n", _lotes[indexLote]->doses_aplicadas);

    if (_lotes[indexLote]->doses_aplicadas == 0) {
        // Se não houve inoculações, remover completamente
        for (int i = 0; i < _numRegistos; i++) {
            if (_registos[i]->lote == _lotes[indexLote]) {
                free(_registos[i]->entrada);
                free(_registos[i]);
                for (int j = i; j < _numRegistos - 1; j++) {
                    _registos[j] = _registos[j + 1];
                }
                _numRegistos--;
                break;
            }
        }

        free(_lotes[indexLote]->nome);
        free(_lotes[indexLote]->lote);
        free(_lotes[indexLote]);

        for (int i = indexLote; i < _numLotes - 1; i++) {
            _lotes[i] = _lotes[i + 1];
        }
        _numLotes--;

    } else {
        // Se houver inoculações, manter o lote com doses_aplicadas
        _lotes[indexLote]->doses_disponiveis = 0;
    }
}

// Função auxiliar para apagar um registo de utente
int apagarRegistoPorIndice(int indice) {
    free(_utentes[indice].nomeUtente);
    free(_utentes[indice].nomeVacina);
    free(_utentes[indice].nomeLote);

    for (int j = indice; j < _numUtentes - 1; j++) {
        _utentes[j] = _utentes[j + 1];
    }

    _numUtentes--;
    return 1; // Retorna 1 para indicar que um registo foi apagado
}

// Função auxiliar para verificar se a data é válida e não futura
int dataValidaNaoFutura(Data* data) {
    if (!dataValida(data)) return 0;
    if (data->ano > _dataActual.ano ||
        (data->ano == _dataActual.ano && data->mes > _dataActual.mes) ||
        (data->ano == _dataActual.ano && data->mes == _dataActual.mes && data->dia > _dataActual.dia)) {
        return 0;
    }
    return 1;
}

// Função auxiliar para apagar registos por utente e data
int apagarRegistosPorUtenteEData(const char* nomeUtente, Data* data) {
    int totalApagados = 0;
    for (int i = 0; i < _numUtentes; ) {
        if (strcmp(_utentes[i].nomeUtente, nomeUtente) == 0 &&
            _utentes[i].dataVacinacao.dia == data->dia &&
            _utentes[i].dataVacinacao.mes == data->mes &&
            _utentes[i].dataVacinacao.ano == data->ano) {
            totalApagados += apagarRegistoPorIndice(i);
        } else {
            i++;
        }
    }
    return totalApagados;
}

// Função auxiliar para apagar registos por utente, data e lote
int apagarRegistosPorUtenteDataELote(const char* nomeUtente, Data* data, const char* lote) {
    int totalApagados = 0;
    for (int i = 0; i < _numUtentes; ) {
        if (strcmp(_utentes[i].nomeUtente, nomeUtente) == 0 &&
            _utentes[i].dataVacinacao.dia == data->dia &&
            _utentes[i].dataVacinacao.mes == data->mes &&
            _utentes[i].dataVacinacao.ano == data->ano &&
            strcmp(_utentes[i].nomeLote, lote) == 0) {
            totalApagados += apagarRegistoPorIndice(i);
        } else {
            i++;
        }
    }
    return totalApagados;
}

// Função principal refatorada
void apagarAplicacoesPT() {
    char nomeUtenteTemp[200000];
    leEspacos();
    leNome(nomeUtenteTemp);

    int encontrouUtente = 0;

    if (leEspacos()) {
        char dataTemp[MAX_CHAR_DATA];
        leNome(dataTemp);

        Data* dataApagar = converteTextoData(dataTemp);
        if (!dataValidaNaoFutura(dataApagar)) {
            printf("data inválida\n");
            free(dataApagar);
            return;
        }

        if (leEspacos()) {
            char loteTemp[200000];
            leNome(loteTemp);

            int totalApagados = apagarRegistosPorUtenteDataELote(nomeUtenteTemp, dataApagar, loteTemp);
            free(dataApagar);

            if (totalApagados == 0) {
                printf("%s: lote inexistente\n", loteTemp);
            } else {
                printf("%d\n", totalApagados);
            }
            
        } else {
            int totalApagados = apagarRegistosPorUtenteEData(nomeUtenteTemp, dataApagar);
            free(dataApagar);

            if (totalApagados == 0) {
                printf("data inválida\n");
            } else {
                printf("%d\n", totalApagados);
            }
        }

    } else {
        int totalApagados = 0;
        for (int i = 0; i < _numUtentes; ) {
            if (strcmp(_utentes[i].nomeUtente, nomeUtenteTemp) == 0) {
                encontrouUtente = 1;
                totalApagados += apagarRegistoPorIndice(i);
            } else {
                i++;
            }
        }

        if (!encontrouUtente) {
            printf("%s: utente inexistente\n", nomeUtenteTemp);
        } else {
            printf("%d\n", totalApagados);
        }
    }
}

void listarAplicacoesPT() {
    if (leEspacos()) {
        char nomeUtente[200000]; // Supondo um tamanho máximo para o nome
        leNome(nomeUtente); // Lê o nome do utente

        int encontrou = 0;

        // Percorre todos os registos de utentes em busca do nome fornecido
        for (int i = 0; i < _numUtentes; i++) {
            if (strcmp(_utentes[i].nomeUtente, nomeUtente) == 0) {
                printf("%s %s %02d-%02d-%04d\n",
                       _utentes[i].nomeUtente,
                       _utentes[i].nomeVacina,
                       _utentes[i].dataVacinacao.dia,
                       _utentes[i].dataVacinacao.mes,
                       _utentes[i].dataVacinacao.ano);
                encontrou = 1;
            }
        }

        // não encontrou nenhum registo para o utente
        if (!encontrou) {
            printf("%s: utente inexistente\n", nomeUtente);
        }

    } else {
        // Lista todas as aplicações do sistema em ordem cronológica
        if (_numUtentes == 0) {
            return; // Se não há aplicações, não imprime nada
        }

        // Ordena os registos por data antes de exibir
        for (int i = 0; i < _numUtentes - 1; i++) {
            for (int j = i + 1; j < _numUtentes; j++) {
                if ((_utentes[i].dataVacinacao.ano > _utentes[j].dataVacinacao.ano) ||
                    (_utentes[i].dataVacinacao.ano == _utentes[j].dataVacinacao.ano &&
                     _utentes[i].dataVacinacao.mes > _utentes[j].dataVacinacao.mes) ||
                    (_utentes[i].dataVacinacao.ano == _utentes[j].dataVacinacao.ano &&
                     _utentes[i].dataVacinacao.mes == _utentes[j].dataVacinacao.mes &&
                     _utentes[i].dataVacinacao.dia > _utentes[j].dataVacinacao.dia)) {
                    
                    // Troca os registos para ordenar corretamente
                    Utente temp = _utentes[i];
                    _utentes[i] = _utentes[j];
                    _utentes[j] = temp;
                }
            }
        }

        // Exibe todas as aplicações de vacina em ordem cronológica
        for (int i = 0; i < _numUtentes; i++) {
            printf("%s %s %02d-%02d-%04d\n",
                   _utentes[i].nomeUtente,
                   _utentes[i].nomeVacina,
                   _utentes[i].dataVacinacao.dia,
                   _utentes[i].dataVacinacao.mes,
                   _utentes[i].dataVacinacao.ano);
        }
    }
}

void avancaTempoPT(){
    char data[MAX_CHAR_DATA];
    if (leEspacos()){
      leNome(data);
      Data *novaData = converteTextoData(data);
      if (dataValida(novaData)) {
        // Verifica se a nova data é posterior ou igual à data atual
        if (novaData->ano > _dataActual.ano ||
            (novaData->ano == _dataActual.ano && novaData->mes > _dataActual.mes) ||
            (novaData->ano == _dataActual.ano && novaData->mes == _dataActual.mes && novaData->dia >= _dataActual.dia)) {
            actualizaDataActual(novaData);
            printf("%02d-%02d-%02d\n", novaData->dia, novaData->mes,novaData->ano);
            free(novaData);
        } else {
            printf("data inválida\n"); // Data anterior à atual, inválida no contexto do sistema
            free(novaData);
        }
    } else {
        printf("data inválida\n"); 
        free(novaData);
    }
    }else{
      printf("%02d-%02d-%02d\n", _dataActual.dia, _dataActual.mes, _dataActual.ano);
      return;
    }
    
    
}

void processaComandoPT(int d) {
    switch (d) {
        case 'c':
            insereLotePT();
            break;
        case 'l': {
            char argumentos[200000];
            leAteFinalLinha(argumentos);
            listarLotesPT(strlen(argumentos) > 0 ? argumentos : NULL);
            break;
        }
        case 'a':
            aplicarDosePT();
            break;
        case 'r':
            retiraDisponibilidadePT();
            break;
        case 'd':
            apagarAplicacoesPT();
            break;
        case 'u':
            listarAplicacoesPT();
            break;
        case 't':
            avancaTempoPT();
            break;
        default:
            if (d == ' ' || d == '\t' || d == '\n') 
                break;
            /* Se não começar com um caracter válido, descarta a linha inteira */
            leAteFinalLinha(NULL);
            break;
    }
}

void processaComandoEN(int c){
    switch(c) {
        case 'c':
            insereLote();
            break;
        case 'l': {
            char argumentos[200000]; // Buffer para armazenar os argumentos
            leAteFinalLinha(argumentos); // Lê o restante da linha como argumentos
            listarLotes(strlen(argumentos) > 0 ? argumentos : NULL); // Passa argumentos se existirem
            break;
        }
        case 'a': 
            aplicarDose();
            break;
        case 'r':
            retiraDisponibilidade();
            break;
        case 'd':
            apagarAplicacoes();
            break;
        case 'u':
            listarAplicacoes();
            break;
        case 't':
            avancaTempo();
            break;
        default:
            /* Ignorar linhas em branco */
            if (c == ' ' || c == '\t' || c == '\n') 
                break;
            /* Se não começar com um caracter válido, descarta a linha inteira */
            leAteFinalLinha(NULL);
            break;
    }
}

int main(int argc, char *argv[]) {
    int d;
    inicializarData();
    if (argc > 1 && strcmp(argv[1], "pt") == 0) {  // Comparar o argumento com "pt"
        do {
            d = getchar();           
            processaComandoPT(d);
        } while (d != 'q');
        libertaLotes();
        libertaRegistos();
        libertaUtentes();
        return 0;
    } else {
        int c;
        do {
            c = getchar();
            processaComandoEN(c);
        } while (c != 'q');
        libertaLotes();
        libertaRegistos();
        libertaUtentes();
        return 0;
    }
}
