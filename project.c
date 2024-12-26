/*Projeto de Introdução aos Algoritmos e estruturas de Dados
Henrique Pires Lhano
Número: 109281
henrique.lhano@tecnico.ulisboa.pt*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "proj1.h" // Ficheiro Header


int main() {
    char c, y;
    while ((c = getchar()) != 'q') {
        switch (c) {
            case 'p':
                y = getchar();
                if (y == EOF || y == '\n') {
                    ungetc(y, stdin);
                    mostraparques();
                } else {
                    ungetc(y, stdin);
                    criaparque();
                }
                break;
            case 'e':
                adicionaveiculo();
                break;
            case 's':
                removeveiculo();
                break;
            case 'v':
                listamovimentos();
                break;
            case 'f':
                mostrafaturacao2();
                break;
            case 'r':
                removerparque();
                break;
        }
        while (getchar() != '\n'); // Limpa o buffer de entrada
    }
    return 0;
}
 /* O comando f tem dois casos, que seriam feitos de uma maneira deste género. 
    contudo, não os consegui acabar de aplicar, por isso ficam apenas definidas as funcoes de faturacao para cada um dos casos. 
    ungetc(c, stdin);
    f (comandof() == 1){
    c = getchar();
    mostrafaturacao();
    };
    if (comandof() == 2){
    c = getchar();
    mostrafaturacao2();
    };
*/
void criaparque(){
    int i;
    char nome[BUFSIZ];
    int capacidade;
    float valor_15, valor_15_apos_1hora, valor_max;
    char c;
    // Verifica se o próximo caractere é uma aspa
    if (scanf(" %c", &c) == 1 && c == '\"') {
        // Se houver aspas, lê o nome entre aspas até encontrar outra aspa
        scanf("%[^\"]\"", nome);
    } else {
        // Caso contrário, lê o nome normalmente sem espaços
        ungetc(c, stdin); // Devolve o caractere lido de volta ao buffer
        scanf("%s", nome);
    }
    scanf("%d %f %f %f", &capacidade, &valor_15, &valor_15_apos_1hora, &valor_max);
    for (i = 0; i < numero_parques ; i++){
        if(strcmp(Parques[i].nome, nome)==0){
            printf("%s: %s\n", nome, erro_1);
            return;
        }
    }
    if (capacidade <=0 ){
        printf("%d: %s\n", capacidade, erro_2);
        return;
    }
    if (valor_15 <=0 || valor_15_apos_1hora <= 0 || 
        valor_max <=0 || valor_15_apos_1hora<=valor_15 || 
        valor_max <= valor_15_apos_1hora || valor_max <= valor_15){
        printf("%s\n",erro_3);
        return;
    }
    if (numero_parques >= MAXPARQUE){
        printf("%s\n", erro_4);
        return;
    }
    strcpy(Parques[numero_parques].nome, nome);
    Parques[numero_parques].capacidade = capacidade;
    Parques[numero_parques].valor_15 = valor_15;
    Parques[numero_parques].valor_15_apos_1hora = valor_15_apos_1hora;
    Parques[numero_parques].valor_max = valor_max;
    Parques[numero_parques].lugares_disponiveis = capacidade;
    numero_parques++;

}
void mostraparques() {
    for (int i = 0; i < numero_parques; i++) {
        if (strlen(Parques[i].nome) > 0) {
            printf("%s %d %d\n", Parques[i].nome,
                   Parques[i].capacidade,
                   Parques[i].lugares_disponiveis);
        }
    }
}
void adicionaveiculo() {
    char nome[BUFSIZ], matricula[MAXMATRICULA], c;
    data dia_entrada;
    hora a_entrada;

    // Ler o nome do parque
    if (scanf(" %c", &c) == 1 && c == '\"') {
        // Se houver aspas, lê o nome entre aspas até encontrar outra aspa
        scanf("%[^\"]\"", nome);
    } else {
        // Caso contrário, lê o nome normalmente sem espaços
        ungetc(c, stdin); // Devolve o caractere lido de volta ao buffer
        scanf("%s", nome);
    }
    int indice_parque = encontrar_indice_parque(nome);
    if (indice_parque == -1) {
        printf("%s: %s\n", nome, erro_5);
        return;
    }
    if (Parques[indice_parque].lugares_disponiveis == 0) {
        printf("%s: %s\n", nome, erro_6);
        return;
    }
    scanf("%s", matricula); 
    scanf("%d-%d-%d %d:%d", &dia_entrada.dia, &dia_entrada.mes, &dia_entrada.ano, &a_entrada.horas, &a_entrada.minutos);
    for (int i = 0; i < numero_parques; i++) {
        CARRO *temp_veiculo = Parques[i].lista_veiculos;
        while (temp_veiculo != NULL) {
            if (strcmp(temp_veiculo->matricula, matricula) == 0) {
                printf("%s: %s\n", matricula, erro_8); // Veículo já estacionado
                return;
            }
            temp_veiculo = temp_veiculo->next;
        }
    }
    if (!e_data_valida(dia_entrada)){
        printf("%s\n", erro_9);
        return;
    }
    if (!e_hora_valida(a_entrada)){
        printf("%s\n", erro_9);
        return;
    }
    // Verificar se a hora de entrada é posterior à hora de entrada dos veículos já registados
    CARRO *temp = Parques[indice_parque].lista_veiculos;
    while (temp != NULL) {
        if (temp->dia_entrada.ano == dia_entrada.ano && temp->dia_entrada.mes == dia_entrada.mes &&
            temp->dia_entrada.dia == dia_entrada.dia && 
            (temp->a_entrada.horas > a_entrada.horas || 
             (temp->a_entrada.horas == a_entrada.horas && temp->a_entrada.minutos > a_entrada.minutos))) {
            printf("%s\n", erro_9); // Hora de entrada inválida
            return;
        }
        temp = temp->next;
    }
    if (!e_matricula_valida(matricula)){
        printf("%s: %s\n", matricula, erro_7);
        return;
    }
    // Criar um novo veículo
    CARRO *novo_carro = (CARRO *)malloc(sizeof(CARRO));
    if (novo_carro == NULL) {
        return;
    }
    strcpy(novo_carro->matricula, matricula);
    novo_carro->dia_entrada = dia_entrada;
    novo_carro->a_entrada = a_entrada;
    novo_carro->next = NULL;

    // Adicionar o novo veículo à lista de veículos do parque
    if (Parques[indice_parque].lista_veiculos == NULL) {
        Parques[indice_parque].lista_veiculos = novo_carro;
    } else {
        CARRO *temp = Parques[indice_parque].lista_veiculos;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = novo_carro;
    }
    // Atualizar o número de lugares disponíveis no parque
    Parques[indice_parque].lugares_disponiveis--;
    printf("%s %d\n", nome, Parques[indice_parque].lugares_disponiveis);
    //Adicionar ao histórico de entradas
    MOVIMENTO *movimento_entrada = (MOVIMENTO *)malloc(sizeof(MOVIMENTO));
    if (movimento_entrada == NULL) {
        return;
    }
    strcpy(movimento_entrada->matricula, matricula);
    movimento_entrada->dia_entrada = dia_entrada;
    movimento_entrada->a_entrada = a_entrada;
    movimento_entrada->deu_saida.horas = 0; // Indicador de que o veículo ainda está no parque
    movimento_entrada->next = NULL;
    // Adiciona o novo movimento à lista de movimentos do parque
    if (Parques[indice_parque].lista_movimentos == NULL) {
        Parques[indice_parque].lista_movimentos = movimento_entrada;
    } else {
        MOVIMENTO *temp = Parques[indice_parque].lista_movimentos;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = movimento_entrada;
    }
}
void removeveiculo() {
    char nome[BUFSIZ], matricula[MAXMATRICULA], c;
    data dia_saida;
    hora a_saida;

    // Ler o nome do parque
    if (scanf(" %c", &c) == 1 && c == '\"') {
        scanf("%[^\"]\"", nome);
    } else {
        ungetc(c, stdin);
        scanf("%s", nome);
    }
    int indice_parque = encontrar_indice_parque(nome);
    if (indice_parque == -1) {
        printf("%s: %s\n", nome, erro_5);
        return;
    }
    scanf("%s", matricula); 
    scanf("%d-%d-%d %d:%d", &dia_saida.dia, &dia_saida.mes, &dia_saida.ano, &a_saida.horas, &a_saida.minutos);

    // Verificar se a matrícula é válida
    if (!e_matricula_valida(matricula)){
        printf("%s\n", erro_7);
        return;
    }
    if (!e_data_valida(dia_saida) || !e_hora_valida(a_saida)) {
        printf("%s\n", erro_9);
        return;
    }

    // Verificar se o veículo está dentro do parque especificado
    CARRO *temp = Parques[indice_parque].lista_veiculos;
    CARRO *ant = NULL;
    while (temp != NULL) {
        if (strcmp(temp->matricula, matricula) == 0) {
            // Verificar se a data/hora de saída é posterior à data/hora de entrada
            // e a todas as outras operações de entrada/saída registadas no parque
            CARRO *temp2 = Parques[indice_parque].lista_veiculos;
            while (temp2 != NULL) {
                if (temp2->deu_saida.horas == 0 && // Verificar se o veículo ainda está no parque
                    (temp2->dia_entrada.ano > dia_saida.ano ||
                     (temp2->dia_entrada.ano == dia_saida.ano && temp2->dia_entrada.mes > dia_saida.mes) ||
                     (temp2->dia_entrada.ano == dia_saida.ano && temp2->dia_entrada.mes == dia_saida.mes && temp2->dia_entrada.dia > dia_saida.dia) ||
                     (temp2->dia_entrada.ano == dia_saida.ano && temp2->dia_entrada.mes == dia_saida.mes && temp2->dia_entrada.dia == dia_saida.dia && temp2->a_entrada.horas > a_saida.horas) ||
                     (temp2->dia_entrada.ano == dia_saida.ano && temp2->dia_entrada.mes == dia_saida.mes && temp2->dia_entrada.dia == dia_saida.dia && temp2->a_entrada.horas == a_saida.horas && temp2->a_entrada.minutos > a_saida.minutos))) {
                    printf("%s\n", erro_9);
                    return;
                }
                temp2 = temp2->next;
            }
            break;
        }
        ant = temp;
        temp = temp->next;
    }
    // Se o veículo não está no parque
    if (temp == NULL) {
        printf("%s: %s\n", matricula, erro_10);
        return;
    }

    MOVIMENTO *movimento = (MOVIMENTO *)malloc(sizeof(MOVIMENTO));
    if (movimento == NULL) {
        return;
    }
    strcpy(movimento->matricula, temp->matricula);
    movimento->dia_entrada = temp->dia_entrada;
    movimento->a_entrada = temp->a_entrada;
    movimento->deu_saida = a_saida; 
    movimento->a_saida = dia_saida; 
    movimento->next = NULL;
    // Adiciona o novo movimento à lista de movimentos do parque
    if (Parques[indice_parque].lista_movimentos == NULL) {
        Parques[indice_parque].lista_movimentos = movimento;
    } else {
        MOVIMENTO *temp = Parques[indice_parque].lista_movimentos;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = movimento;
    }
    // Calcular o valor a ser pago pelo veículo
    float valor_pago = calcula_valor(temp->dia_entrada, dia_saida, temp->a_entrada, a_saida,
                                      Parques[indice_parque].valor_15, Parques[indice_parque].valor_15_apos_1hora,
                                      Parques[indice_parque].valor_max);
    // Registar a saída do veículo
    temp->deu_saida = a_saida; 
    temp->a_saida = dia_saida;
    Parques[indice_parque].lugares_disponiveis++;
    printf("%s %02d-%02d-%04d %02d:%02d %02d-%02d-%02d %02d:%02d %.2f\n", temp->matricula, temp->dia_entrada.dia, temp->dia_entrada.mes,
           temp->dia_entrada.ano, temp->a_entrada.horas, temp->a_entrada.minutos, dia_saida.dia, dia_saida.mes,
           dia_saida.ano, a_saida.horas, a_saida.minutos, valor_pago);
    // Remover o veículo da lista de veículos do parque
    if (ant == NULL) {
        Parques[indice_parque].lista_veiculos = temp->next;
    } else {
        ant->next = temp->next;
    }
    free(temp);
}

int apenasimpar(char matricula[MAXMATRICULA], MOVIMENTO *subtemp) {
    int contador = 0;
    while (subtemp != NULL){
        if (strcmp(subtemp->matricula, matricula) == 0){
            contador = contador + 1;
        }
        subtemp = subtemp->next;  
    }
    return contador;
}

//Função que permite executar o comando v
void listamovimentos() {
    char matricula[MAXMATRICULA];
    scanf("%s", matricula);
    if (!e_matricula_valida(matricula)){
        printf("%s: %s\n", matricula, erro_7);
        return;
    }    

    int movimentos_encontrados = 0; // Variável para controlar se foram encontrados movimentos
    // Percorrer todos os parques
    for (int i = 0; i < numero_parques; i++) {
        MOVIMENTO *temp = Parques[i].lista_movimentos;
        MOVIMENTO *subtemp = Parques[i].lista_movimentos;
        while (temp != NULL) {
            if (strcmp(temp->matricula, matricula) == 0) {
                if (temp->deu_saida.horas != 0) {
                    printf("%s %02d-%02d-%04d %02d:%02d %02d-%02d-%02d %02d:%02d\n", Parques[i].nome, temp->dia_entrada.dia, temp->dia_entrada.mes,
                           temp->dia_entrada.ano, temp->a_entrada.horas, temp->a_entrada.minutos, temp->a_saida.dia, temp->a_saida.mes,
                           temp->a_saida.ano, temp->deu_saida.horas, temp->deu_saida.minutos);
                }                      
                if (temp->deu_saida.horas == 0 && apenasimpar(matricula, subtemp)%2 == 1 && apenasimpar(matricula,temp) == 1) {
                    
                    printf("%s %02d-%02d-%04d %02d:%02d\n", Parques[i].nome, temp->dia_entrada.dia, temp->dia_entrada.mes,
                           temp->dia_entrada.ano, temp->a_entrada.horas, temp->a_entrada.minutos);
                }
                movimentos_encontrados = 1;
            }

            temp = temp->next;
        }
    }
    if (!movimentos_encontrados) {
        printf("%s: %s\n", matricula, erro_11);
    }
}



void mostrafaturacao() {   // Com a entrada f <nome-parque>
    char nome[BUFSIZ], c;
    // Ler o nome do parque
    if (scanf(" %c", &c) == 1 && c == '\"') {
        scanf("%[^\"]\"", nome);
    } else {
        ungetc(c, stdin);
        scanf("%s", nome);
    }
    int indice_parque = encontrar_indice_parque(nome);
    if (indice_parque == -1) {
        printf("%s: %s\n", nome, erro_5);
        return;
    }
    // Array para armazenar a faturação diária
    MOVIMENTO *originalmovimento = Parques[indice_parque].lista_movimentos;
    MOVIMENTO *movimento = Parques[indice_parque].lista_movimentos;
    // Calcular a faturação diária
    while (movimento != NULL) {
        if (movimento->deu_saida.horas != 0) {
            data datateste = {movimento->a_saida.dia, movimento->a_saida.mes, movimento->a_saida.ano};
            if (datanvezes(datateste, originalmovimento) == datanvezes(datateste, movimento)){
                int j = datanvezes(datateste, originalmovimento);
                float faturacao_diaria = 0;
                MOVIMENTO *submov = movimento;
                while (j > 0 && submov != NULL) {
                    if (datateste.ano == submov->a_saida.ano && datateste.mes == submov->a_saida.mes && datateste.dia == submov->a_saida.dia){
                        float valor = calcula_valor(submov->dia_entrada, submov->a_saida, 
                                                submov->a_entrada, submov->deu_saida,
                                                Parques[indice_parque].valor_15, 
                                                Parques[indice_parque].valor_15_apos_1hora, 
                                                Parques[indice_parque].valor_max);
                        faturacao_diaria += valor; 
                        submov = submov->next;
                        j = j-1;
                    } else{
                        submov = submov->next;
                    }
                }
                printf("%02d-%02d-%04d %.2f\n", datateste.dia, datateste.mes, datateste.ano, faturacao_diaria);
            }
        }
        movimento = movimento->next;
    }
}
// Conta quantas vezes a data aparece na estrutura MOVIMENTO
int datanvezes(data dataver, MOVIMENTO *mov) {
    int contador = 0;
    while (mov != NULL){
        if (dataver.ano == mov->a_saida.ano && dataver.mes == mov->a_saida.mes && dataver.dia == mov->a_saida.dia){
            contador = contador + 1;
        }
        mov = mov->next;  
    }
    return contador;
}
void mostrafaturacao2(){
    data dataacalcular;
    char nome[BUFSIZ], c;
    if (scanf(" %c", &c) == 1 && c == '\"') {
        scanf("%[^\"]\"", nome);
    } else {
        ungetc(c, stdin);
        scanf("%s", nome);
    }
    int indice_parque = encontrar_indice_parque(nome);
    if (indice_parque == -1) {
        printf("%s: %s\n", nome, erro_5);
        return;
    }
    scanf("%d-%d-%d", &dataacalcular.dia, &dataacalcular.mes, &dataacalcular.ano);
    if (!e_data_valida(dataacalcular)) {
        printf("%s\n", erro_9);
        return;
    }
    MOVIMENTO *movimento = Parques[indice_parque].lista_movimentos;
    while (movimento != NULL){
        if (dataacalcular.dia == movimento->a_saida.dia && dataacalcular.mes == movimento->a_saida.mes 
            && dataacalcular.ano == movimento->a_saida.ano){
            float valor = calcula_valor(movimento->dia_entrada, movimento->a_saida, 
                                                    movimento->a_entrada, movimento->deu_saida,
                                                    Parques[indice_parque].valor_15, 
                                                    Parques[indice_parque].valor_15_apos_1hora, 
                                                    Parques[indice_parque].valor_max);
            printf("%s %02d:%02d %.2f\n", movimento->matricula, movimento->deu_saida.horas, movimento->deu_saida.minutos, valor);
        }
        movimento = movimento->next;        
    }
}

void removerparque() {
    char c;
    char nome[BUFSIZ];
    if (scanf(" %c", &c) == 1 && c == '\"') {
        scanf("%[^\"]\"", nome);
    } else {
        ungetc(c, stdin);
        scanf("%s", nome);
    }
    int indice = encontrar_indice_parque(nome);
    if (indice == -1) {
        printf("%s: %s\n", nome, erro_5);
        return;
    }
    
    // Liberar memória alocada para os movimentos associados ao parque
    MOVIMENTO *temp_mov = Parques[indice].lista_movimentos;
    while (temp_mov != NULL) {
        MOVIMENTO *remover_mov = temp_mov;
        temp_mov = temp_mov->next;
        free(remover_mov); // Libera a memória alocada para os registos de movimentos
    }
    
    // Liberar memória alocada para os veículos associados ao parque
    CARRO *temp_car = Parques[indice].lista_veiculos;
    while (temp_car != NULL) {
        CARRO *remover_car = temp_car;
        temp_car = temp_car->next;
        free(remover_car); // Libera a memória alocada para os registos de veículos
    }
    
    // Remove o parque do sistema
    for (int i = indice; i < numero_parques - 1; i++) {
        Parques[i] = Parques[i + 1];
    }
    numero_parques--;
    
    // Exibe os parques restantes ordenados pelo nome
    for (int i = 0; i < numero_parques; i++) {
        printf("%s\n", Parques[i].nome);
    }
}















int encontrar_indice_parque(char nome[BUFSIZ]){
    for (int i = 0; i < numero_parques; i++){
        if (strcmp(Parques[i].nome, nome) == 0) {
            return i; // Retorna o índice se encontrar o parque
        }
    }
    return -1; // Retorna -1 se não encontrar o parque
}

int diasnoMes(int mes) {
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
int e_data_valida(data dataaverificar) {
    if (dataaverificar.mes < 1 || dataaverificar.mes > 12) {
        return 0; // Mês inválido
    }
    if (dataaverificar.dia < 1 || dataaverificar.dia > diasnoMes(dataaverificar.mes)) {
        return 0; // Dia inválido
    }
    if (dataaverificar.mes == 2 && ((dataaverificar.ano % 4 == 0 && dataaverificar.ano % 100 != 0) || dataaverificar.ano % 400 == 0)) {
        if (dataaverificar.dia > 29) {
            return 0; // Dia inválido em ano bissexto
        }
    }
    return 1; // Data válida
}
int e_hora_valida(hora horaverificar) {
    if (horaverificar.horas < 0 || horaverificar.horas > 23) {
        return 0; // Horas inválidas
    }
    if (horaverificar.minutos < 0 || horaverificar.minutos > 59) {
        return 0; // Minutos inválidos
    }
    return 1; // Hora válida
}
int minutosentreDatas(data dataentrada, data datasaida, hora horaentrada, hora horasaida) {
    int ndias = 0;
    int dif_anos = datasaida.ano - dataentrada.ano;
    int nminutos = 0;
    int dif_meses = datasaida.mes - dataentrada.mes;
    int dif_dias = datasaida.dia - dataentrada.dia;

    if (dif_anos == 0){
        if (dif_meses == 0){
            if (dif_dias == 0){
                nminutos = (horasaida.horas - horaentrada.horas)*60 + (horasaida.minutos - horaentrada.minutos);
            }
            if (dif_dias >= 1){
                //numero de dias inteiros completos entre o dia de entrada e de saida
                ndias = ndias + (dif_dias - 1);
                //numero de horas e minutos no dia de saida e no dia de entrada
                nminutos = nminutos + horasaida.horas*60 + horasaida.minutos;
                nminutos = nminutos + 60*(24 - horaentrada.horas-1) + (60 - horaentrada.minutos);
                // adicionar os dias completos 
                nminutos = nminutos + ndias*24*60;
            }
            
        }
        if (dif_meses >= 1){
            if (dif_meses > 1){
                for (int mess = (dataentrada.mes + 1); mess < datasaida.mes; mess++){
                    ndias = ndias + diasnoMes(mess);
                } 
            }
            // numero de dias completo no mes de saida
            ndias = ndias + datasaida.dia - 1;
            // numero de dias completos no mes de entrada
            ndias = ndias + diasnoMes(dataentrada.mes) - dataentrada.dia;
            //numero de horas e minutos no dia de saida e no dia de entrada
            nminutos = nminutos + horasaida.horas*60 + horasaida.minutos;
            nminutos = nminutos + 60*(24 - horaentrada.horas-1) + (60 - horaentrada.minutos);
            // adicionar os dias completos 
            nminutos = nminutos + ndias*24*60;
        }        
    }

    if (dif_anos >= 1){
        // numero de dias em anos completos
        ndias = ndias + (dif_anos - 1)*365;
        // numero de dias no ano de saida nos meses completos
        for (int mes = 1; mes < datasaida.mes; mes++){
            ndias = ndias + diasnoMes(mes);
        }
        // numero de dias completos no ano de saida no ultimo mes 
        ndias = ndias + datasaida.dia - 1; 

        // numero de dias em meses completos no ano de entrada
        for (int mes_e = (dataentrada.mes + 1); mes_e <= 12; mes_e++){
            ndias = ndias + diasnoMes(mes_e);
        }
        // numero de dias completos no mes de entrada 
        ndias = ndias + diasnoMes(dataentrada.mes) - dataentrada.dia;
        // converter para min
        nminutos = nminutos + ndias*24*60;
        nminutos = nminutos + 60*horasaida.horas + horasaida.minutos;
        nminutos = nminutos + 60*(24 - horaentrada.horas-1) + (60 - horaentrada.minutos);
        
    }
    return nminutos;
}

float calcula_valor(data dataentrada, data datasaida, hora horaentrada, hora horasaida, 
                    float valor_15, float valor_15_apos_1hora, float valor_max){
    int n_minutos = minutosentreDatas(dataentrada, datasaida, horaentrada, horasaida);
    int periodos_24inteiros = n_minutos/(24*60);
    int min_restantes = n_minutos%(24*60);
    float valor_pagar = 0;
    int periodos_restantes = 0;
    float valor_periodos = 0;
    
    valor_pagar = valor_pagar + valor_max*periodos_24inteiros;
    if (min_restantes == 0){
        return valor_pagar;
    }
    if (min_restantes%15 > 0){
        periodos_restantes = min_restantes/15 + 1;
    }
    if (min_restantes%15 ==0){
        periodos_restantes = min_restantes/15;
    }
    if (periodos_restantes <= 4){
        valor_periodos = periodos_restantes*valor_15;
    }
    if (periodos_restantes > 4){
        valor_periodos = 4*valor_15 + (periodos_restantes - 4)*valor_15_apos_1hora;
    }
    if (valor_periodos >= valor_max){
        valor_pagar = valor_pagar + valor_max;
    }
    if (valor_periodos < valor_max){
        valor_pagar = valor_pagar + valor_periodos;
    }
    return valor_pagar;
}


int e_matricula_valida(char matricula[]) {
    int letras = 0, digitos = 0;
    // Verificar se a entrada não é uma string vazia e que tem o tamanho certo
    if (strlen(matricula) != 8){
        return 0;
    }
    // Verificar se os pares estao bem divididos por um "-"
    if (matricula[2] != '-' || matricula[5] != '-'){
        return 0;
    }

    // Verificar que todos os elementos dos pares sao letra ou digito e se os caracteres no mesmo par são letras e se sim maiúsculas
    for (int i = 0, j = 1; (i < 7 && j < 8); i += 3, j += 3){
        if (!isalpha(matricula[i]) && !isdigit(matricula[i])){
            return 0;
        }
        if (!isalpha(matricula[j]) && !isdigit(matricula[j])){
            return 0;
        }
        if ((isdigit(matricula[i]) && isalpha(matricula[j])) || (isalpha(matricula[i]) && isdigit(matricula[j]))) {
            return 0;
        }
        if (isalpha(matricula[i]) && isalpha(matricula[j])) {
            if(!isupper(matricula[i]) || !isupper(matricula[j])){
                return 0;
            }
            letras++;  
        }
        if (isdigit(matricula[i]) && isdigit(matricula[j])) {
            digitos++;
        }
    }
    // Verificar se há pelo menos um par de letras e um par de dígitos
    if (letras == 0 || digitos == 0){
        return 0;
    }
    return 1; // Matrícula válida
}