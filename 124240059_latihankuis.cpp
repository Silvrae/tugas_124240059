#include <iostream>
#include <fstream>

using namespace std;

struct Penduduk {
    int nik;
    char nama[50];
    char alamat[100];
    char jenis_kelamin;
};

void tambahdata() {
    Penduduk data;
    FILE *file = fopen("datapenduduk1.dat", "ab");

    if (!file) {
        cout << "gagal membuka file" << endl;
        return;
    }
    cout << "masukkan NIK: ";
    cin >> data.nik;
    cin.ignore();
    cout << "masukkan nama: ";
    cin.getline(data.nama, 50);
    cout << "masukkan alamat: ";
    cin.getline(data.alamat, 100);
    cout << "masukkan jenis Kelamin (L/P): ";
    cin >> data.jenis_kelamin;
    fwrite(&data, sizeof(Penduduk), 1, file);
    fclose(file);
    cout << "data berhasil disimpan\n";
}
void tampilkandata() {
    FILE *file = fopen("datapenduduk1.dat", "rb");
    if (!file) {
        cout << "error\n";
        return;
    }
    Penduduk data[100];
    int jumlah = 0;
    while (fread(&data[jumlah], sizeof(Penduduk), 1, file)) {
        jumlah++;
    }
    fclose(file);
    if (jumlah == 0) {
        cout << "data kosong\n";
        return;
    }
    for (int i = 0; i < jumlah - 1; i++) {
        for (int j = 0; j < jumlah - i - 1; j++) {
            if (data[j].nik > data[j + 1].nik) {
                swap(data[j], data[j + 1]);
            }
        }
    }
    cout << " ---- data penduduk ----\n";
    for (int i = 0; i < jumlah; i++) {
        cout << "NIK            : " << data[i].nik << endl;
        cout << "nama           : " << data[i].nama << endl;
        cout << "alamat         : " << data[i].alamat << endl;
        cout << "jenis kelamin  : " << data[i].jenis_kelamin << endl;
        cout << "-----------------------------\n";
    }
}

void caridata() {
    FILE *file = fopen("datapenduduk1.dat", "rb");
    if (!file) {
        cout << "error\n";
        return;
    }

    int carinik;
    cout << "masukkan NIK yang ingin dicari: ";
    cin >> carinik;

    Penduduk p;
    bool ditemukan = false;

    while (fread(&p, sizeof(Penduduk), 1, file)) {
        if (p.nik == carinik) {
            ditemukan = true;
            cout << " --- DATA PENDUDUK ---\n";
            cout << "NIK         : " << p.nik << endl;
            cout << "nama        : " << p.nama << endl;
            cout << "alamat      : " << p.alamat << endl;
            cout << "jenis Kelamin: " << p.jenis_kelamin << endl;
            cout << "-----------------------------\n";
            break;
        }
    }

    if (!ditemukan) {
        cout << " NIK " << carinik << " tidak ditemukan\n";
    }

    fclose(file);
}

void editdata() {
    FILE *file = fopen("datapenduduk1.dat", "rb+");
    if (!file) {
        cout << "error\n";
        return;
    }

    int carinik;
    cout << "masukkan NIK yang akan diedit: ";
    cin >> carinik;
    cin.ignore();
    Penduduk p;
    bool ditemukan = false;
    long pos = 0;

    while (fread(&p, sizeof(Penduduk), 1, file)) {
        if (p.nik == carinik) {
            ditemukan = true;
            cout << "--- data lama ---\n";
            cout << "NIK         : " << p.nik << endl;
            cout << "nama        : " << p.nama << endl;
            cout << "alamat      : " << p.alamat << endl;
            cout << "jenis Kelamin: " << p.jenis_kelamin << endl;
            cout << "-----------------------------\n";
            cout << "masukkan alamat baru: ";
            cin.getline(p.alamat, 100);
            fseek(file, pos, SEEK_SET);
            fwrite(&p, sizeof(Penduduk), 1, file);

            cout << "alamat berhasil diperbarui\n";
            break;
        }
        pos = ftell(file);
    }
    if (!ditemukan) {
        cout << "NIK " << carinik << " tidak ditemukan\n";
    }
    fclose(file);
}

void hapusdata() {
    FILE *file = fopen("datapenduduk1.dat", "rb");
    if (!file) {
        cout << "error\n";
        return;
    }
    FILE *temp = fopen("temp.dat", "wb");
    int hapusdata;
    bool ditemukan = false;
    cout << "masukkan NIK yang ingin dihapus: ";
    cin >> hapusdata;
    Penduduk p;
    while (fread(&p, sizeof(Penduduk), 1, file)) {
        if (p.nik == hapusdata) {
            ditemukan = true;
            continue;
        }
        fwrite(&p, sizeof(Penduduk), 1, temp);
    }
    fclose(file);
    fclose(temp);
    remove("datapenduduk1.dat");
    rename("temp.dat", "datapenduduk1.dat");
    if (ditemukan)
        cout << "data berhasil dihapus\n";
    else
        cout << "NIK " << hapusdata << " tidak ditemukan\n";
}

int main() {
    int pilihan;
    do {
        cout << "\nProgram Kantor Kecamatan\n";
        cout << "1. input data\n";
        cout << "2. tampilkan data\n";
        cout << "3. search data\n";
        cout << "4. edit data\n";
        cout << "5. hapus data\n";
        cout << "6. exit\n";
        cout << "masukkan pilihan Anda: ";
        cin >> pilihan;

        cin.ignore();

        switch (pilihan) {
            case 1:
                tambahdata();
                break;
            case 2:
                tampilkandata();
                break;
            case 3:
                caridata();
                break;
            case 4:
               editdata();
                break;
            case 5:
                hapusdata();
                break;
            case 6:
                cout << "terima kasih!";
                break;
            default:
                cout << "plihan tidak valid\n";
                break;
        }
    } while (pilihan != 6);

    return 0;
}
