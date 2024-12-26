/*
 * pro1.h Estruturas e funções protótipo para o projeto 
 */

#ifndef PROJ1_H
#define PROJ1_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h> 

/*-----Erros e constantes-----*/
#define erro_1 "parking already exists."
#define erro_2 "invalid capacity."
#define erro_3 "invalid cost."
#define erro_4 "too many parks."
#define erro_5 "no such parking."
#define erro_6 "parking is full."
#define erro_7 "invalid licence plate."
#define erro_8 "invalid vehicle entry."
#define erro_9 "invalid date."
#define erro_10 "invalid vehicle exit."
#define erro_11 "no entries found in any parking."
#define MAXPARQUE 20
#define MAXMATRICULA 9


typedef struct {
    int dia, mes, ano;
} data;

typedef struct {
    int horas, minutos;
} hora;

typedef struct carro {
    char matricula[MAXMATRICULA];
    data dia_entrada;
    hora a_entrada;
    hora deu_saida;
    data a_saida;
    struct carro *next;
} CARRO;

typedef struct movimento {
    char matricula[MAXMATRICULA];
    data dia_entrada;
    hora a_entrada;
    hora deu_saida;
    data a_saida;
    struct movimento *next;
} MOVIMENTO;


typedef struct {
    char nome[BUFSIZ];
    int capacidade, lugares_disponiveis;
    float valor_15, valor_15_apos_1hora, valor_max;
    CARRO *lista_veiculos; // Lista encadeada de veículos
    MOVIMENTO *lista_movimentos;
} parque;



/*------Variáveis Globais------*/
parque Parques[MAXPARQUE];
int numero_parques = 0;
CARRO *numero_carros = NULL; // O número de veículos inicial é 0


/*--------Funções protótipo--------*/
void criaparque();
void mostraparques();
void adicionaveiculo();
int e_matricula_valida(char matricula[]);
int e_data_valida(data dataaverificar);
int e_hora_valida(hora horaverificar);
int encontrar_indice_parque(char nome[BUFSIZ]);
void removerparque();
void removeveiculo();
int minutosentreDatas(data dataentrada, data datasaida, hora horaentrada, hora horasaida);
int diasnoMes(int mes);
void listamovimentos();
float calcula_valor(data dataentrada, data datasaida, hora horaentrada, hora horasaida, 
                    float valor_15, float valor_15_apos_1hora, float valor_max);
void mostrafaturacao();
void mostrafaturacao2();
int datanvezes(data dataver, MOVIMENTO *mov);

#endif