
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

enum Status { TERSEDIA, DALAM_ANTREAN, SEDANG_DIPUTAR };
enum AksiType { TAMBAH, HAPUS, PLAYLIST, TONTON };

string statuskestring(Status s) {
    switch (s) {
        case TERSEDIA: return "tersedia";
        case DALAM_ANTREAN: return "dalam_antrian";
        case SEDANG_DIPUTAR: return "sedang_diputar";
    }
    return "";
}

Status stringkestatus(const string& s) {
    if (s == "tersedia") return TERSEDIA;
    if (s == "dalam_antrian") return DALAM_ANTREAN;
    if (s == "sedang_diputar") return SEDANG_DIPUTAR;
    return TERSEDIA;
}

struct Video {
    string judul;
    int durasi;
    Status status;
};

struct TreeNode {
    Video data;
    TreeNode* left;
    TreeNode* right;
};

struct Playlist {
    Video* video;
    Playlist* next;
};

struct Riwayat {
    Video* video;
    Riwayat* next;
};

struct UndoItem {
    AksiType tipe;
    Video data;
};

struct NodeUndo {
    UndoItem aksi;
    NodeUndo* next;
};

TreeNode* root = nullptr;
Playlist* headPlaylist = nullptr;
Riwayat* topRiwayat = nullptr;
NodeUndo* topUndo = nullptr;
TreeNode* insert(TreeNode* node, Video video) {
    if (!node) return new TreeNode{video, nullptr, nullptr};
    if (video.judul < node->data.judul) node->left = insert(node->left, video);
    else if (video.judul > node->data.judul) node->right = insert(node->right, video);
    return node;
}

TreeNode* carivideo(TreeNode* node, const string& judul) {
    if (!node) return nullptr;
    if (judul == node->data.judul) return node;
    if (judul < node->data.judul) return carivideo(node->left, judul);
    return carivideo(node->right, judul);
}

bool carijudul(TreeNode* node, const string& judul) {
    if (!node) return false;
    if (node->data.judul == judul) return true;
    if (judul < node->data.judul) return carijudul(node->left, judul);
    return carijudul(node->right, judul);
}

TreeNode* cariterkecil(TreeNode* node) {
    while (node && node->left) node = node->left;
    return node;
}

TreeNode* hapusdariBST(TreeNode* node, const string& judul) {
    if (!node) return nullptr;
    if (judul < node->data.judul) node->left = hapusdariBST(node->left, judul);
    else if (judul > node->data.judul) node->right = hapusdariBST(node->right, judul);
    else {
        if (!node->left) {
            TreeNode* temp = node->right;
            delete node;
            return temp;
        } else if (!node->right) {
            TreeNode* temp = node->left;
            delete node;
            return temp;
        } else {
            TreeNode* temp = cariterkecil(node->right);
            node->data = temp->data;
            node->right = hapusdariBST(node->right, temp->data.judul);
        }
    }
    return node;
}

void inorder(TreeNode* node) {
    if (!node) return;
    inorder(node->left);
    cout << "Judul: " << node->data.judul << ", Durasi: " << node->data.durasi
         << ", Status: " << statuskestring(node->data.status) << endl;
    inorder(node->right);
}

bool inplaylist(const string& judul) {
    Playlist* temp = headPlaylist;
    while (temp) {
        if (temp->video->judul == judul) return true;
        temp = temp->next;
    }
    return false;
}

void tambahkePlaylist(Video* v) {
    if (inplaylist(v->judul)) {
        cout << "video sudah ada ato sendang diputar\n";
        return;
    }

    v->status = (headPlaylist == nullptr) ? SEDANG_DIPUTAR : DALAM_ANTREAN;
    Playlist* baru = new Playlist{v, nullptr};
    if (!headPlaylist) headPlaylist = baru;
    else {
        Playlist* temp = headPlaylist;
        while (temp->next) temp = temp->next;
        temp->next = baru;
    }

    cout << "video ditambahkan ke playlist\n";
}

void hapusDariPlaylist(const string& judul) {
    Playlist* prev = nullptr;
    Playlist* curr = headPlaylist;
    while (curr) {
        if (curr->video->judul == judul) {
            if (!prev) headPlaylist = curr->next;
            else prev->next = curr->next;
            delete curr;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void tambahriwayat(Video* v) {
    Riwayat* baru = new Riwayat{v, topRiwayat};
    topRiwayat = baru;
}

void tampikanriwayat() {
    if (!topRiwayat) {
        cout << "tidak ada riwayat\n";
        return;
    }
    Riwayat* temp = topRiwayat;
    cout << "\nRiwayat Tontonan\n";
    while (temp) {
        cout << "Judul: " << temp->video->judul << ", Durasi: " << temp->video->durasi << endl;
        temp = temp->next;
    }
}

void simpankeFile(TreeNode* node, ofstream& output) {
    if (!node) return;
    simpankeFile(node->left, output);
    output << node->data.judul << "," << node->data.durasi << "," << statuskestring(node->data.status) << endl;
    simpankeFile(node->right, output);
}

void muatDariFile() {
    ifstream input("video.txt");
    if (!input.is_open()) return;
    string judul, statusStr;
    int durasi;
    while (getline(input, judul, ',') && input >> durasi && input.ignore() && getline(input, statusStr)) {
        Video v{judul, durasi, stringkestatus(statusStr)};
        root = insert(root, v);
    }
    input.close();
}

void simpankePlaylist() {
    ofstream output("playlist.txt");
    Playlist* temp = headPlaylist;
    while (temp) {
        output << temp->video->judul << endl;
        temp = temp->next;
    }
    output.close();
}

void masukPlaylistkefile() {
    ifstream input("playlist.txt");
    if (!input.is_open()) return;
    string judul;
    while (getline(input, judul)) {
        TreeNode* node = carivideo(root, judul);
        if (node) tambahkePlaylist(&node->data);
    }
    input.close();
}

void simpanRiwayatkeFile() {
    ofstream output("riwayat.txt");
    Riwayat* temp = topRiwayat;
    while (temp) {
        output << temp->video->judul << endl;
        temp = temp->next;
    }
    output.close();
}

void muatRiwayatdarifile() {
    ifstream input("riwayat.txt");
    if (!input.is_open()) return;
    string judul;
    while (getline(input, judul)) {
        TreeNode* node = carivideo(root, judul);
        if (node) tambahriwayat(&node->data);
    }
    input.close();
}

void undo(UndoItem aksi) {
    NodeUndo* baru = new NodeUndo{aksi, topUndo};
    topUndo = baru;
}

void undo() {
    if (!topUndo) {
        cout << "tidak ada aksi\n";
        return;
    }
    UndoItem aksi = topUndo->aksi;
    NodeUndo* temp = topUndo;
    topUndo = topUndo->next;
    delete temp;

    if (aksi.tipe == TAMBAH) root = hapusdariBST(root, aksi.data.judul);
    else if (aksi.tipe == HAPUS) root = insert(root, aksi.data);
    else if (aksi.tipe == PLAYLIST) {
        hapusDariPlaylist(aksi.data.judul);
        TreeNode* node = carivideo(root, aksi.data.judul);
        if (node) node->data.status = TERSEDIA;
    }
    else if (aksi.tipe == TONTON) {
        if (topRiwayat && topRiwayat->video->judul == aksi.data.judul) {
            Riwayat* temp = topRiwayat;
            topRiwayat = topRiwayat->next;
            delete temp;
        }
        TreeNode* node = carivideo(root, aksi.data.judul);
        if (node) tambahkePlaylist(&node->data);
    }

    ofstream output("video.txt");
    simpankeFile(root, output);
    output.close();
    simpankePlaylist();
    simpanRiwayatkeFile();

    cout << "Undo berhasil.\n";
}

void tontonVideo() {
    if (!headPlaylist) {
        cout << "Playlist kosong\n";
        return;
    }
    Playlist* ditonton = headPlaylist;
    headPlaylist = headPlaylist->next;
    ditonton->video->status = TERSEDIA;
    cout << "Sedang menonton: " << ditonton->video->judul << endl;
    tambahriwayat(ditonton->video);
    delete ditonton;
}

int main() {
    muatDariFile();
    masukPlaylistkefile();
    muatRiwayatdarifile();

    int pilihan;
    do {
        cout << "\nMenu:\n";
        cout << "1. tambah video\n";
        cout << "2. tampilkan daftar video\n";
        cout << "3. tambahkan ke playlist\n";
        cout << "4. tonton video\n";
        cout << "5. riwayat tontonan\n";
        cout << "6. hapus video\n";
        cout << "7. undo tindakan terakhir\n";
        cout << "0. keluar\n";
        cout << "pilih: ";
        cin >> pilihan;
        cin.ignore();
        
        Video v;
        string key;
        TreeNode* t = nullptr;
        char c, yn;
        switch (pilihan) {
            case 1:
                cout << "judul: ";
                getline(cin, v.judul);
                if (carijudul(root, v.judul)) {
                    cout << "judul sudah ada\n";
                    break;
                }
                cout << "durasi (menit): ";
                cin >> v.durasi; cin.ignore();
                v.status = TERSEDIA;
                root = insert(root, v);
                undo({TAMBAH, v});
                {
                    ofstream output("video.txt");
                    simpankeFile(root, output);
                    output.close();
                }
                break;
            case 2:
                inorder(root);
                cout << "cari video (y/t)? ";
                cin >> c; cin.ignore();
                if (c == 'y' || c == 'Y') {
                    cout << "masukkan judul: ";
                    getline(cin, key);
                    t = carivideo(root, key);
                    if (t) cout << "video ditemukan: " << t->data.judul << " - " << statuskestring(t->data.status) << endl;
                    else cout << "video tidak ditemukan\n";
                }
                break;
            case 3:
                cout << "judul: ";
                getline(cin, key);
                t = carivideo(root, key);
                if (!t) {
                    cout << "judul tidak ditemukan\n";
                } else {
                    undo({PLAYLIST, t->data});
                    tambahkePlaylist(&t->data);
                    simpankePlaylist();
                }
                break;
            case 4:
                if (headPlaylist)
                    undo({TONTON, *headPlaylist->video});
                tontonVideo();
                simpanRiwayatkeFile();
                simpankePlaylist();
                break;
            case 5:
                tampikanriwayat();
                break;
            case 6:
                cout << "judul: ";
                getline(cin, key);
                t = carivideo(root, key);
                if (!t) {
                    cout << "judul tidak ditemukan\n";
                    break;
                }
                if (t->data.status != TERSEDIA) {
                    cout << "video sedang " << statuskestring(t->data.status) << ". yakin hapus? (y/t): ";
                    cin >> yn; cin.ignore();
                    if (yn != 'y' && yn != 'Y') break;
                }
                undo({HAPUS, t->data});
                hapusDariPlaylist(key);
                root = hapusdariBST(root, key);
                {
                    ofstream output("video.txt");
                    simpankeFile(root, output);
                    output.close();
                }
                simpankePlaylist();
                cout << "video dihapus\n";
                break;
            case 7:
                undo();
                break;
            case 0:
                cout << "keluar...\n";
                break;
            default:
                cout << "pilihan tidak valid\n";
        }
    } while (pilihan != 0);
    return 0;
}
