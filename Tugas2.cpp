#include <iostream>
#include <fstream>

using namespace std;

struct toko
{
    string nama;
    string roti;
    int harga;
    toko *prev;
    toko *next;
};

    toko *head = nullptr;
    toko *tail = nullptr;

    toko *riwayat_head = nullptr;
    toko *riwayat_tail = nullptr;

    toko* buatpesanan(string nama, string roti, int harga) {
        toko *baru = new toko;
        baru->nama = nama;
        baru->roti = roti;
        baru->harga = harga;
        baru->prev = nullptr;
        baru->next = nullptr;
        return baru;
    }

    void simpanantrian(){
        ofstream file ("antri.txt");
        toko  *bantu= head;
        while (bantu != nullptr){
            file << bantu->nama << "," << bantu->roti << "," << bantu->harga << endl;
            bantu = bantu->next;
        }
        file.close();
    }

    void simpanriwayat() {
        ofstream file ("riwayat.txt");
        toko *bantu = riwayat_head;
        while (bantu != nullptr) {
            file << bantu->nama << "," << bantu->roti << "," << bantu->harga << endl;
            bantu = bantu->next;
        }
        file.close();
    }

    void pesanan (){
        string nama, roti;
        int harga;
        cout << "Nama pembeli: ";
        getline(cin, nama);
        cout << "Roti yang dipesan: ";
        getline(cin, roti);
        cout << "Harga roti: ";
        cin >> harga;
        cin.ignore();
        toko *baru = buatpesanan(nama, roti, harga);
        if (head == nullptr)
        {
            head = tail = baru;
        }
        else
        {
            tail->next = baru;
            baru->prev = tail;
            tail = baru;
        }
        simpanantrian();
        cout << "pesanan berhasil ditambahkan\n";
    }

    void tampilkanantrian(){
        if ( head == nullptr){
            cout << "antrian kosong\n";
            return;
        }
        toko *bantu = head;
        int i = 1;
        cout << "\ndaftar antrian:\n";
        while (bantu != nullptr){
            cout << i++ << ". " << bantu->nama << " - " << bantu->roti << " - Rp" << bantu->harga << endl;
            bantu = bantu->next;   
        }
    }

    void layanan(){
        if (head == nullptr){
            cout << "antrian kosong\n";
            return;
        }
        toko *helep = head;
        cout << "pesanan dilayani\n";
        cout << "- Nama       : " << head->nama << endl;
        cout << "- Roti       : " << head->roti << endl;
        cout << "- Total Harga: " << head->harga << endl;
        toko *baru = buatpesanan(head->nama, head->roti, head->harga);
        if (riwayat_head == nullptr){
            riwayat_head = riwayat_tail = baru;
        }
        else{
            riwayat_tail->next = baru;
            baru->prev = riwayat_tail;
            riwayat_tail = baru;
        }    
        head = head->next;
        if (head != nullptr) 
            head->prev = nullptr;
       else 
            tail = nullptr;

        delete helep;
        simpanantrian();
        simpanriwayat();

    }

    void batalkanpesananakhir(){
        if (tail == nullptr){
            cout << "antrian kosong\n";
            return ;
        }

        cout << "membatalkan pesanan terakhir\n";
        cout << "- Nama       : " << tail->nama << endl;
        cout << "- Roti       : " << tail->roti << endl;
        cout << "- Total Harga: " << tail->harga << endl;
        toko *hapus = tail;
        tail = tail->prev;
        if (tail != nullptr) 
            tail->next = nullptr;
        else 
            head = nullptr;
            delete hapus;
        simpanantrian();
        cout << "pesanan terakhir dibatalkan\n";

    } 

    void tampilkanriwayat(){
        if (riwayat_head == nullptr){
            cout << "belum ada riwayat pesanan\n";
            return;
        }
        toko *bantu = riwayat_head;
        int i = 1;
        cout << "\nriwayat pesanan\n";
        while (bantu != nullptr){
            cout << i++ << ". " << bantu->nama << " - " << bantu->roti << " - Rp" << bantu->harga << endl;
            bantu = bantu->next;   
        }
    }
    int main (){
        int pilihan;
        do
        {
        
        cout << "\nsistem antrean toko manis lejat\n";
        cout << "1. ambil antrian\n";
        cout << "2. layani pelanggan\n";
        cout << "3. tampilkan antrean\n";
        cout << "4. tampilkan riwayat\n";
        cout << "5. batalkan pesanan terakhir\n";
        cout << "6. keluar\n";
        cout << "masukkan pilihan anda: ";
        cin >> pilihan;
        cin.ignore();
        
        
            switch (pilihan)
            {
            case 1:
                cout << "anda memilih ambil antrian\n";
                pesanan();
                break;
            case 2:
                cout << "anda memilih layani pelanggan\n";
                layanan();
                break;
            case 3:
                cout << "anda memilih tampilkan antrean\n";
                tampilkanantrian();
                break;
            case 4:
                cout << "anda memilih tampilkan riwayat\n";
                tampilkanriwayat();
                break;
            case 5:
                cout << "anda memilih batalkan pesanan terakhir\n";
                batalkanpesananakhir();
                break;
            case 6:
                cout << "anda memilih keluar\n";
                break;
            default:
                cout << "pilihan tidak valid\n";
                break;
            }
        } while (pilihan != 6);

    }