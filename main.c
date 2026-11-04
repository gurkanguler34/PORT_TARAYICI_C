#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    char hedef_ip[100];
    int baslangic_port, bitis_port;
    int soket_tanimlayici;
    struct sockaddr_in hedef_adresi;

    // Kullanıcıdan hedef bilgileri alıyoruz
    printf("=== C DILI ILE PORT TARAYICI ===\n\n");
    printf("Hedef IP adresini girin (Ornek: 127.0.0.1): ");
    scanf("%99s", hedef_ip);

    printf("Baslangic portunu girin (Ornek: 75): ");
    scanf("%d", &baslangic_port);

    printf("Bitis portunu girin (Ornek: 85): ");
    scanf("%d", &bitis_port);

    printf("\n[%s] adresi, %d-%d portlari arasinda taraniyor...\n\n", hedef_ip, baslangic_port, bitis_port);

    // Belirtilen port aralığını döngü ile tarıyoruz
    for (int mevcut_port = baslangic_port; mevcut_port <= bitis_port; mevcut_port++) {
        
        // 1. Soket Oluşturma (IPv4 ve TCP protokolü kullanılıyor)
        soket_tanimlayici = socket(AF_INET, SOCK_STREAM, 0);
        
        if (soket_tanimlayici < 0) {
            printf("Hata: Soket olusturulamadi!\n");
            return 1;
        }

        // 2. Hedef Adres Bilgilerini Hazırlama
        memset(&hedef_adresi, 0, sizeof(hedef_adresi)); // Belleği temizle
        hedef_adresi.sin_family = AF_INET;              // IPv4 yapısı
        hedef_adresi.sin_port = htons(mevcut_port);     // Port numarasını ağ formatına çevir
        
        // Metin halindeki IP adresini ikili (binary) formata çeviriyoruz
        if (inet_pton(AF_INET, hedef_ip, &hedef_adresi.sin_addr) <= 0) {
            printf("Hata: Gecersiz IP adresi girdiniz!\n");
            close(soket_tanimlayici);
            return 1;
        }

        // 3. Bağlantı İsteği Gönderme (Port Açık mı Kapalı mı Kontrolü)
        // connect() fonksiyonu 0 dönerse port AÇIK, -1 dönerse KAPALIDIR.
        if (connect(soket_tanimlayici, (struct sockaddr *)&hedef_adresi, sizeof(hedef_adresi)) == 0) {
            printf("[+] PORT ACIK: %d\n", mevcut_port);
        } else {
            // İsteğe bağlı olarak kapalı portları da yazdırabilirsiniz:
            // printf("[-] Port Kapali: %d\n", mevcut_port);
        }

        // 4. Soketi Kapatma (Her kontrolden sonra soketi kapatmalıyız)
        close(soket_tanimlayici);
    }

    printf("\nTaram işlemi tamamlandi.\n");
    return 0;
}

