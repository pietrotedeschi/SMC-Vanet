const int Nc=15;
const int Ni=10;
const int Ns=13;

clock time;

typedef int[0,Nc-1] id_c;
typedef int[0,Ni-1] id_i;
typedef int[0,Ns-1] id_s;

int [0,Nc] count=0;
int settings;

const int speedmax_way=40;
const int slowmax_way=15;
const int dist=100;

/*Matrice Intersezione-Segmenti*/
const int Map[id_i][4]= {
{6,10,11,-1},
...
};
{2,3,7,-1  },
{0,4,12,-1 },
{3,12,-1,-1},
{0,11,-1,-1},
{2,8,-1,-1 },
{1,9,-1,-1 },
...
};

/*Matrice Segmento-Intersezioni-0=dx e alto,1=sx e basso*/
const int nInters[id_s][2]={
{6,4},{9,8},
...
};
{0,2},{2,3},
{1,7},{8,1},
{9,0},{0,6},
{4,5}
};

/*Struttura per gestire e memorizzare le coordinate delle Intersezioni*/
typedef struct
{
    int Px;
    int Py;
} Int;

Int I[Ni]={
{1000,1000},
...
};
{1000,0   },
{0,400    },
{0,0      },
{0,1000   },
{1500,0   },
{2000,400 },
{2000,1000}
};
/*Vengono istanziate Ni strutture di tipo Int poichè Ni sono le intersezioni alle quali dobbiamo assegnare le coordinate*/

/*Struttura per gestire e memorizzare le proprietà del Segmento (Lunghezza, Numero dei veicoli corsia SX-DX)*/
typedef struct
{
int length;
int nc_l;
int nc_r;
bool expressway;
} Str;

Str S[Ns]= {
{600,0,0,0 },
...
};
{1000,0,0,1},
{1000,0,0,0},
{500,0,0,0 },
{600,0,0,0 },
{400,0,0,0 },
{400,0,0,0 },
{500,0,0,1 },
{1000,0,0,1},
{1000,0,0,0},
{400,0,0,1 }
};
/*Vengono instanziate le proprietà per ogni Segmento Ns*/
/**FINE Costruzione MAPPA*/

/**INIZIO Costruzione MODELLO di Trasmissione*/
broadcast chan msg[Nc]; broadcast chan alert[Ni];

const int GEN=2; const int REL=3;

/*Contenuto del messaggio trasmesso*/
typedef struct
{
int type;    
...
int VxA; int VyA;     
} Msg;

Msg Mgen[Nc]; Msg Mrec[Nc]; Msg RSUAnomaly[Ni];
int RR=400;
/**FINE Costruzione MODELLO di Trasmissione*/

/**INIZIO Impostazioni veicoli*/

const int r=250;
const int slow_speed=6;
const int mean_speed=20;
const int high_speed=50;
const int BP=7;

/**FINE Impostazioni veicoli*/

/**INIZIO Funzione ABS*/
int abs(int value)
{
if (value<0) value=-value; return value;
}
/**FINE Funzione ABS*/

/**INIZIO Distanza di Manhattan*/
int distance(int x1,int y1, int x2, int y2)
{
return abs(x1-x2) + abs(y1-y2);
}
/**FINE Distanza di Manhattan*/

dynamic RSU(const id_i idR, int idc);
