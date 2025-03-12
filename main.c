#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HASTA 100

// Aciliyet seviyeleri (Enum)
typedef enum {YESIL = 1, SARI = 2, KIRMIZI = 3} Aciliyet;

// Hasta bilgileri (Struct)
typedef struct {
    int id;
    char tc_kimlik_no[12];
    char ad[50];
    int yas;
    Aciliyet seviye;
    char doktor_ad[50];
    int doktor_id;
    char tani[100];
    int recete_no;
} Hasta;

// Kuyruk dugumu
typedef struct Dugum {
    Hasta hasta;
    struct Dugum* sonraki;
} Dugum;

// Kuyruk 
typedef struct {
    Dugum* bas;
    Dugum* son;
} Kuyruk;

Kuyruk hasta_kuyrugu = {NULL, NULL};
int mevcut_id = 1000;
int mevcut_recete_no = 5000;

// Aciliyet seviyesini stringe cevirme
const char* aciliyet_str(Aciliyet ac) {
    switch(ac) {
        case YESIL: return "YESIL";
        case SARI: return "SARI";
        case KIRMIZI: return "KIRMIZI";
        default: return "BILINMEYEN";
    }
}

// T.C. Kimlik No dogrulama fonksiyonu
int tc_kontrol(const char* tc) {
    return strlen(tc) == 11;
}

// Hastayi kuyruga ekleme (Oncelige gore yerlestirme)
void hasta_ekle() {
    Hasta yeni;
    yeni.id = mevcut_id++;

    do {
        printf("T.C. Kimlik Numarasini girin (11 haneli): ");
        scanf("%s", yeni.tc_kimlik_no);
        if (!tc_kontrol(yeni.tc_kimlik_no)) {
            printf("Gecersiz T.C. Kimlik Numarasi! Lutfen tekrar deneyin.\n");
        }
    } while (!tc_kontrol(yeni.tc_kimlik_no));

    printf("Hasta adini girin: ");
    scanf("%s", yeni.ad);

    printf("Hasta yasini girin: ");
    scanf("%d", &yeni.yas);

    printf("Aciliyet seviyesini girin (1=YESIL, 2=SARI, 3=KIRMIZI): ");
    int ac;
    scanf("%d", &ac);
    yeni.seviye = (Aciliyet)ac;

    printf("Doktor adini girin: ");
    scanf("%s", yeni.doktor_ad);

    printf("Doktor ID girin: ");
    scanf("%d", &yeni.doktor_id);

    printf("Taniyi girin: ");
    scanf("%s", yeni.tani);

    yeni.recete_no = mevcut_recete_no++;

    // Yeni dugum olustur
    Dugum* yeni_dugum = (Dugum*)malloc(sizeof(Dugum));
    yeni_dugum->hasta = yeni;
    yeni_dugum->sonraki = NULL;

    // Kuyruga ekleme (Aciliyet onceligine gore siralama)
    if (hasta_kuyrugu.bas == NULL || yeni.seviye > hasta_kuyrugu.bas->hasta.seviye) {
        // Basa ekleme (en acil hasta)
        yeni_dugum->sonraki = hasta_kuyrugu.bas;
        hasta_kuyrugu.bas = yeni_dugum;
        if (hasta_kuyrugu.son == NULL) {
            hasta_kuyrugu.son = yeni_dugum;
        }
    } else {
        // Oncelige gore uygun yere ekleme
        Dugum* mevcut = hasta_kuyrugu.bas;
        while (mevcut->sonraki != NULL && mevcut->sonraki->hasta.seviye >= yeni.seviye) {
            mevcut = mevcut->sonraki;
        }
        yeni_dugum->sonraki = mevcut->sonraki;
        mevcut->sonraki = yeni_dugum;
        if (mevcut == hasta_kuyrugu.son) {
            hasta_kuyrugu.son = yeni_dugum;
        }
    }

    printf("Hasta basariyla eklendi! Hasta ID: %d, Recete No: %d\n", yeni.id, yeni.recete_no);
}

// Hastalari listeleme
void hasta_listele() {
    if (hasta_kuyrugu.bas == NULL) {
        printf("Kayitli hasta bulunmamaktadir.\n");
        return;
    }

    printf("\n--- Hasta Listesi (Oncelik Sirasina Gore) ---\n");
    Dugum* gecici = hasta_kuyrugu.bas;
    while (gecici != NULL) {
        printf("ID: %d | T.C. No: %s | Ad: %s | Yas: %d | Aciliyet: %s | Doktor: %s | Doktor ID: %d | Tani: %s | Recete No: %d\n",
            gecici->hasta.id, gecici->hasta.tc_kimlik_no, gecici->hasta.ad, gecici->hasta.yas, aciliyet_str(gecici->hasta.seviye),
            gecici->hasta.doktor_ad, gecici->hasta.doktor_id, gecici->hasta.tani, gecici->hasta.recete_no);
        gecici = gecici->sonraki;
    }
}

// Hasta silme (Belirtilen ID'ye sahip hastayi silme)
void hasta_sil() {
    if (hasta_kuyrugu.bas == NULL) {
        printf("Silinecek hasta yok!\n");
        return;
    }

    int silinecek_id;
    printf("Silmek istediginiz hasta ID'sini girin: ");
    scanf("%d", &silinecek_id);

    Dugum* onceki = NULL;
    Dugum* mevcut = hasta_kuyrugu.bas;

    while (mevcut != NULL && mevcut->hasta.id != silinecek_id) {
        onceki = mevcut;
        mevcut = mevcut->sonraki;
    }

    if (mevcut == NULL) {
        printf("Belirtilen ID'ye sahip hasta bulunamadi!\n");
        return;
    }

    if (onceki == NULL) {
        hasta_kuyrugu.bas = mevcut->sonraki;
    } else {
        onceki->sonraki = mevcut->sonraki;
    }

    if (mevcut == hasta_kuyrugu.son) {
        hasta_kuyrugu.son = onceki;
    }

    printf("Hasta ID %d silindi.\n", mevcut->hasta.id);
    free(mevcut);
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

// Ana fonksiyon
int main() {
    menu();
    return 0;
}

