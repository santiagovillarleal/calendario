#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define __USE_XOPEN
#include <time.h>

#define ERR_OK                     0
#define ERR_USO                   -1
#define ERR_FECHA                 -2
#define ERR_DIA_SEMANA_NON_VALIDO -3
#define ERR_ANO_MINIMO            -4
#define ERR_ANO_MAXIMO            -5

#define ANO_MINIMO 1829
#define ANO_MAXIMO 2100
#define NUM_FIL_MATRIZ_MESES 28
#define NUM_COL_MATRIZ_MESES 12

void uso(char *prog)
{
  printf("%s <DD/MM/AAAA>\n", prog);
}

int obter_ano(struct tm fecha)
{
  return fecha.tm_year+1900;
}

int obter_mes(struct tm fecha)
{
  return fecha.tm_mon+1;
}

int obter_dia(struct tm fecha)
{
  return fecha.tm_mday;
}

void obter_dia_semana_str(int dia_semana, char *dia_semana_str)
{
  char *dia_semana_str_local=(dia_semana==1)?"Domingo":
                             (dia_semana==2)?"Segunda":
                             (dia_semana==3)?"Terza":
                             (dia_semana==4)?"Quarta":
                             (dia_semana==5)?"Quinta":
                             (dia_semana==6)?"Sexta":"Sábado";

  strcpy(dia_semana_str, dia_semana_str_local);
}

char dia_semana_valido(int dia_semana)
{
  return ((dia_semana>=1)&&(dia_semana<=7));
}

char obter_fila_matriz_meses(int ano)
{
  char fila=0;

  if ((ano>=1829)&&(ano<=1899)) fila=(ano%100)%28;
  if (ano==1900) fila=22;
  if ((ano>=1901)&&(ano<=2000)) fila=((ano+4)%100)%28;
  if ((ano>=2001)&&(ano<=2099)) fila=((ano+20)%100)%28;
  if (ano==2100) fila=13;

  fila--; //Matrices 0-based.

  return fila;
}

int obter_dia_semana(struct tm fecha)
{
  char matriz_meses[NUM_FIL_MATRIZ_MESES][NUM_COL_MATRIZ_MESES] = {
   {4,0,0,3,5,1,3,6,2,4,0,2},
   {5,1,1,4,6,2,4,0,3,5,1,3},
   {6,2,2,5,0,3,5,1,4,6,2,4},
   {0,3,4,0,2,5,0,3,6,1,4,6},
   {2,5,5,1,3,6,1,4,0,2,5,0},
   {3,6,6,2,4,0,2,5,1,3,6,1},
   {4,0,0,3,5,1,3,6,2,4,0,2},
   {5,1,2,5,0,3,5,1,4,6,2,4},
   {0,3,3,6,1,4,6,2,5,0,3,5},
   {1,4,4,0,2,5,0,3,6,1,4,6},
   {2,5,5,1,3,6,1,4,0,2,5,0},
   {3,6,0,3,5,1,3,6,2,4,0,2},
   {5,1,1,4,6,2,4,0,3,5,1,3},
   {6,2,2,5,0,3,5,1,4,6,2,4},
   {0,3,3,6,1,4,6,2,5,0,3,5},
   {1,4,5,1,3,6,1,4,0,2,5,0},
   {3,6,6,2,4,0,2,5,1,3,6,1},
   {4,0,0,3,5,1,3,6,2,4,0,2},
   {5,1,1,4,6,2,4,0,3,5,1,3},
   {6,2,3,6,1,4,6,2,5,0,3,5},
   {1,4,4,0,2,5,0,3,6,1,4,6},
   {2,5,5,1,3,6,1,4,0,2,5,0},
   {3,6,6,2,4,0,2,5,1,3,6,1},
   {4,0,1,4,6,2,4,0,3,5,1,3},
   {6,2,2,5,0,3,5,1,4,6,2,4},
   {0,3,3,6,1,4,6,2,5,0,3,5},
   {1,4,4,0,2,5,0,3,6,1,4,6},
   {2,5,6,2,4,0,2,5,1,3,6,1}
  };
;
  char fila_matriz_meses, col_matriz_meses;
  fila_matriz_meses=col_matriz_meses=0;
  int ano=obter_ano(fecha);
  int mes=obter_mes(fecha);
  int dia=obter_dia(fecha);
  int dia_semana=0;

  fila_matriz_meses=obter_fila_matriz_meses(ano);
  col_matriz_meses=mes-1;

  printf("m[%d][%d]=%d\n", fila_matriz_meses,col_matriz_meses,
                           matriz_meses[(int)fila_matriz_meses][(int)col_matriz_meses]);
  printf("dia:%d\n", dia);

  dia_semana=(matriz_meses[(int)fila_matriz_meses][(int)col_matriz_meses]+dia)%7;
  if (!dia_semana) dia_semana=7;

  return dia_semana;
}

void obter_fecha(char *str_fecha, struct tm *fecha)
{
  char *res_strptime=NULL;
  res_strptime=strptime(str_fecha, "%d/%m/%Y", fecha);
  if ((!res_strptime)||(res_strptime[0]))
  {
    printf("A fecha non é válida: %s.\n", str_fecha);
    exit(ERR_FECHA);
  }

  if (obter_ano(*fecha)<ANO_MINIMO)
  {
    printf("O ano non pode ser inferior a %d.\n", ANO_MINIMO);
    exit(ERR_ANO_MINIMO);
  }
  if (obter_ano(*fecha)>ANO_MAXIMO)
  {
    printf("O ano non pode ser superior a %d.\n", ANO_MAXIMO);
    exit(ERR_ANO_MAXIMO);
  }

}


int main(int argc, char *argv[])
{
  struct tm fecha;
  int dia_semana=0; //1-Domingo, 2-Lunes,...
  char *dia_semana_str=(char *)malloc(50);
  if (argc!=2)
  {
    uso(argv[0]);
    exit(ERR_USO);
  }

  obter_fecha(argv[1], &fecha);

  dia_semana=obter_dia_semana(fecha);

  if (!dia_semana_valido(dia_semana))
  {
    printf("Día %d non existe.\n", dia_semana);
    exit(ERR_DIA_SEMANA_NON_VALIDO);
  }

  obter_dia_semana_str(dia_semana, dia_semana_str);

  printf("O %s foi %s.\n", argv[1], dia_semana_str);

  free(dia_semana_str);
  return ERR_OK;
}

