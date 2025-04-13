/*
 * pro1.h Estruturas e funções protótipo para o projeto 
 */

#ifndef PROJ1_H
#define PROJ1_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h> 

#define MAX_CHAR_VACINA 51
#define MAX_REGISTOS 20000
#define MAX_CHAR_DATA 11
#define MAX_LOTES 1000
#define NAO_ENCONTRADO -1
#define MAX_CHAR_LOTE 21



typedef struct {
  int dia;
  int mes;
  int ano;
} Data;


struct RegistoL;
typedef struct RegistoL RegistoLote;

typedef struct {
    char *nome;      
    char *lote;     
    int doses_disponiveis;
    int doses_aplicadas;
    RegistoLote* registo;
} LoteVacina;

struct RegistoL {
  LoteVacina* lote;
  Data* entrada;
};

typedef struct{
  char *nomeUtente;
  char *nomeVacina;
  char *nomeLote;
  Data dataVacinacao;
  int numeroVacinacoes;
} Utente;


int _numLotes = 0;
LoteVacina* _lotes[MAX_LOTES];
Data _dataActual;
RegistoLote* _registos[MAX_REGISTOS];
int _numRegistos = 0;
Utente _utentes[MAX_REGISTOS];
int _numUtentes = 0;
#endif