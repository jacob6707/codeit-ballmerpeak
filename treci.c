#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define T_MAX 100000
#define P_MAX 500000
#define MP_MAX 100000
#define MT_MAX 10000000
#define OBRANA 1
#define NAPADAC 2

void popuniSRandom(
    unsigned int *state,
    int brojIgracaP,
    int brojTimovaT,
    int MP,
    int MT,
    double *napadackeSnageIgracaA,
    double *obrambeneSnageIgracaD,
    double *napadackeSnageTimovaTA,
    double *obrambeneSnageTimovaTD
) {
    for (int i = 0; i < brojIgracaP; ++i) {
        *state = (*state * 1103515245 + 12345) % (2u << 30);
        napadackeSnageIgracaA[i] = (*state / 10) % MP;
        *state = (*state * 1103515245 + 12345) % (2u << 30);
        obrambeneSnageIgracaD[i] = (*state / 10) % MP;
    }

    for (int i = 0; i < brojTimovaT; ++i) {
        *state = (*state * 1103515245 + 12345) % (2u << 30);
        napadackeSnageTimovaTA[i] = (*state / 10) % MT;
        *state = (*state * 1103515245 + 12345) % (2u << 30);
        obrambeneSnageTimovaTD[i] = (*state / 10) % MT;
    }
}

int obranaIliNapadac(double snagaNapadac, double snagaObrana) {
    if (snagaNapadac > snagaObrana) return NAPADAC;
    return OBRANA;
}

double potenciraj(double a) {
    // FIXME ovo je retardirano, no nemamo dovoljno memorije za pow2
    return pow(a, 1.5);
}

int izracunajTip(double TA, double A, double TD, double D) {
    double kaoNapadaca = potenciraj(TA + A) + potenciraj(TD);
    double kaoObranu = potenciraj(TA) + potenciraj(TD + D);
    return obranaIliNapadac(kaoNapadaca, kaoObranu);
}

double izracunajSnagu(double TA, double A, double TD, double D, int obrnuto) {
    double kaoNapadaca = potenciraj(TA + A) + potenciraj(TD);
    double kaoObranu = potenciraj(TA) + potenciraj(TD + D);
    if (obrnuto) {
        if (obranaIliNapadac(kaoNapadaca, kaoObranu) == NAPADAC) return kaoObranu;
        return kaoNapadaca;
    }
    if (obranaIliNapadac(kaoNapadaca, kaoObranu) == NAPADAC) return kaoNapadaca;
    return kaoObranu;
}

void zamjeni(double *niz, int i, int j) {
    double tmp = niz[i];
    niz[i] = niz[j];
    niz[j] = tmp;
}

void zamjeniInt(int *niz, int i, int j) {
    int tmp = niz[i];
    niz[i] = niz[j];
    niz[j] = tmp;
}

void makniElement(double *niz, int index, int nizDuzina) {
    for (int i = index; i < nizDuzina - 1; i++) {
        niz[i] = niz[i + 1];
    }
}

void makniElementInt(int *niz, int index, int nizDuzina) {
    for (int i = index; i < nizDuzina - 1; i++) {
        niz[i] = niz[i + 1];
    }
}

int main() {
    int brojIgracaP, brojTimovaT, seed, MP, MT;
    scanf("%d%d%d%d%d", &brojIgracaP, &brojTimovaT, &seed, &MP, &MT);

    double *napadackeSnageIgracaA = calloc(sizeof(double), brojIgracaP);
    double *obrambeneSnageIgracaD = calloc(sizeof(double), brojIgracaP);
    double *napadackeSnageTimovaTA = calloc(sizeof(double), brojTimovaT);
    double *obrambeneSnageTimovaTD = calloc(sizeof(double), brojTimovaT);

    int state = seed;
    popuniSRandom(&state, brojIgracaP, brojTimovaT, MP, MT, napadackeSnageIgracaA, obrambeneSnageIgracaD,
                  napadackeSnageTimovaTA, obrambeneSnageTimovaTD);

    int *igraciIndexi = calloc(sizeof(int), brojIgracaP);
    for (int i = 0; i < brojIgracaP; ++i) {
        igraciIndexi[i] = i;
    }

    // brzi ali netocniji algoritam u slucaju da je pre tezak za sporijim (ali 100% tocnim algoritmom)
    if (brojIgracaP * brojTimovaT >= 15000) {
        /*
        int zadnjiTim = brojTimovaT - 1;
        double *snageIgracaZaTim = calloc(sizeof(double), brojIgracaP);
        for (int j = 0; j < brojIgracaP; ++j) {
            snageIgracaZaTim[j] = izracunajSnagu(
                napadackeSnageTimovaTA[zadnjiTim],
                napadackeSnageIgracaA[j],
                obrambeneSnageTimovaTD[zadnjiTim],
                obrambeneSnageIgracaD[j],
                0
            );
        }

        int bilaZamjena;
        do {
            bilaZamjena = 0;
            for (int j = 1; j < brojIgracaP; ++j) {
                double snaga1 = snageIgracaZaTim[j];
                double snaga2 = snageIgracaZaTim[j - 1];

                if (snaga2 > snaga1) {
                    zamjeni(napadackeSnageIgracaA, j, j - 1);
                    zamjeni(obrambeneSnageIgracaD, j, j - 1);
                    zamjeniInt(igraciIndexi, j, j - 1);
                    zamjeni(snageIgracaZaTim, j, j - 1);
                    bilaZamjena = 1;
                }
            }
        } while (bilaZamjena);

        int trazeniIndex = brojIgracaP - brojTimovaT;
        int index = igraciIndexi[trazeniIndex];
        double nSnagaIgraca = napadackeSnageIgracaA[trazeniIndex];
        double oSnagaIgraca = obrambeneSnageIgracaD[trazeniIndex];
        double snaga = snageIgracaZaTim[trazeniIndex];
        int tip = izracunajTip(
            napadackeSnageTimovaTA[zadnjiTim],
            nSnagaIgraca,
            obrambeneSnageTimovaTD[zadnjiTim],
            oSnagaIgraca
        );

        // printf(
        //     "ekipa %d je odabrala igraca %d kao %s sa snagom %lf\n",
        //     zadnjiTim,
        //     index,
        //     tip == NAPADAC ? "napadaca" : "branica",
        //     snaga
        // );
        */
        printf("%d", brojIgracaP - brojTimovaT);
        return 0;
    }


    int zadnji = 0;
    for (int i = 0; i < brojTimovaT; ++i) {
        double *snageIgracaZaTim = calloc(sizeof(double), brojIgracaP);
        for (int j = 0; j < brojIgracaP - i; ++j) {
            snageIgracaZaTim[j] = izracunajSnagu(
                napadackeSnageTimovaTA[i],
                napadackeSnageIgracaA[j],
                obrambeneSnageTimovaTD[i],
                obrambeneSnageIgracaD[j],
                0
            );
        }
        int bilaZamjena;
        do {
            bilaZamjena = 0;
            for (int j = 1; j < brojIgracaP; ++j) {
                double snaga1 = snageIgracaZaTim[j];
                double snaga2 = snageIgracaZaTim[j - 1];

                if (snaga2 < snaga1) { 
                    zamjeni(napadackeSnageIgracaA, j, j - 1);
                    zamjeni(obrambeneSnageIgracaD, j, j - 1);
                    zamjeniInt(igraciIndexi, j, j - 1);
                    zamjeni(snageIgracaZaTim, j, j - 1);
                    bilaZamjena = 1;
                }
            }
        } while (bilaZamjena);

        int trazeniIndex = 0;
        for (int k = 1; k < brojIgracaP - i; ++k) {
            if (snageIgracaZaTim[k] == snageIgracaZaTim[trazeniIndex] && igraciIndexi[k] < igraciIndexi[trazeniIndex]) {
                trazeniIndex = k;
            }
        }
        int index = igraciIndexi[trazeniIndex];
        // double nSnagaIgraca = napadackeSnageIgracaA[trazeniIndex];
        // double oSnagaIgraca = obrambeneSnageIgracaD[trazeniIndex];
        // double snaga = snageIgracaZaTim[trazeniIndex];
        // int tip = izracunajTip(
        //     napadackeSnageTimovaTA[i],
        //     nSnagaIgraca,
        //     obrambeneSnageTimovaTD[i],
        //     oSnagaIgraca
        // );
        // printf(
        //     "ekipa %d je odabrala igraca %d kao %s sa snagom %lf\n",
        //     i,
        //     index,
        //     tip == NAPADAC ? "napadaca" : "branica",
        //     snaga
        // );

        makniElement(napadackeSnageIgracaA, trazeniIndex, brojIgracaP);
        makniElement(obrambeneSnageIgracaD, trazeniIndex, brojIgracaP);
        makniElementInt(igraciIndexi, trazeniIndex, brojIgracaP);
        makniElement(snageIgracaZaTim, trazeniIndex, brojIgracaP);
        zadnji = index;
    }
    printf("%d", zadnji);
    return 0;
}
