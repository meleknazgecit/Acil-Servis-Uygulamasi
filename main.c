#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HASTA 100

// Aciliyet seviyeleri
typedef enum {YESIL = 1, SARI = 2, KIRMIZI = 3} Aciliyet;

// Hasta bilgileri
typedef struct {
    int id;
    char ad[50];
    int yas;
    Aciliyet seviye;
    char doktor_ad[50];
    int doktor_id;
    char tani[100];
    int recete_no;
} Hasta;

Hasta hastalar[MAX_HASTA];
int hasta_sayisi = 0;
int mevcut_id = 1000;
int mevcut_recete_no = 5000;

// Aciliyet seviyesini stringe çevirme
const char* aciliyet_str(Aciliyet ac) {
    switch(ac) {
        case YESIL: return "YESIL";
        case SARI: return "SARI";
        case KIRMIZI: return "KIRMIZI";
        default: return "BILINMEYEN";
    }
}

// Yeni hasta ekleme
void hasta_ekle() {
    if (hasta_sayisi >= MAX_HASTA) {
        printf("Hasta kayit kapasitesi dolu!\n");
        return;
    }
    Hasta h;
    h.id = mevcut_id++;
    printf("Hasta adini girin: ");
    scanf("%s", h.ad);
    printf("Hasta yasini girin: ");
    scanf("%d", &h.yas);
    printf("Aciliyet seviyesini girin (1=YESIL, 2=SARI, 3=KIRMIZI): ");
    int ac;
    scanf("%d", &ac);
    h.seviye = (Aciliyet)ac;
    printf("Doktor adini girin: ");
    scanf("%s", h.doktor_ad);
    printf("Doktor ID girin: ");
    scanf("%d", &h.doktor_id);
    printf("Taniyi girin: ");
    scanf("%s", h.tani);
    h.recete_no = mevcut_recete_no++;
    
    hastalar[hasta_sayisi++] = h;
    printf("Hasta basariyla eklendi! Hasta ID: %d, Recete No: %d\n", h.id, h.recete_no);
}

// Hastalari aciliyet seviyesine gore sirala (Bubble Sort)
void hastalari_sirala() {
    for (int i = 0; i < hasta_sayisi - 1; i++) {
        for (int j = 0; j < hasta_sayisi - i - 1; j++) {
            if (hastalar[j].seviye < hastalar[j+1].seviye) {
                Hasta temp = hastalar[j];
                hastalar[j] = hastalar[j+1];
                hastalar[j+1] = temp;
            }
        }
    }
}

// Hasta listesi gosterme
void hasta_listele() {
    if (hasta_sayisi == 0) {
        printf("Kayitli hasta bulunmamaktadir.\n");
        return;
    }
    hastalari_sirala();
    printf("\n--- Hasta Listesi ---\n");
    for (int i = 0; i < hasta_sayisi; i++) {
        printf("ID: %d | Ad: %s | Yas: %d | Aciliyet: %s | Doktor: %s | Doktor ID: %d | Tani: %s | Recete No: %d\n",
            hastalar[i].id, hastalar[i].ad, hastalar[i].yas, aciliyet_str(hastalar[i].seviye),
            hastalar[i].doktor_ad, hastalar[i].doktor_id, hastalar[i].tani, hastalar[i].recete_no);
    }
}

// Hasta silme
void hasta_sil() {
    int id;
    printf("Silmek istediginiz hastanin ID'sini girin: ");
    scanf("%d", &id);
    for (int i = 0; i < hasta_sayisi; i++) {
        if (hastalar[i].id == id) {
            for (int j = i; j < hasta_sayisi - 1; j++) {
                hastalar[j] = hastalar[j+1];
            }
            hasta_sayisi--;
            printf("Hasta ID %d silindi.\n", id);
            return;
        }
    }
    printf("Belirtilen ID ile hasta bulunamadi.\n");
}

// Ana menu
void menu() {
    int secim;
    do {
        printf("\n--- Acil Servis Sistemi ---\n");
        printf("1. Hasta Ekle\n");
        printf("2. Hasta Listele\n");
        printf("3. Hasta Sil\n");
        printf("4. Cikis\n");
        printf("Seciminizi girin: ");
        scanf("%d", &secim);
        switch (secim) {
            case 1: hasta_ekle(); break;
            case 2: hasta_listele(); break;
            case 3: hasta_sil(); break;
            case 4: printf("Cikis yapiliyor...\n"); break;
            default: printf("Gecersiz secim!\n");
        }
    } while (secim != 4);
}

int main() {
    menu();
    return 0;
}

