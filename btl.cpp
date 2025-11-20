#include <bits/stdc++.h>
using namespace std;

class Graph {
private:
    int n;                      // số tài sản
    vector<vector<int>> w;      // ma trận rủi ro
    vector<vector<pair<int,int>>> adj; // danh sách kề: (đỉnh, rủi ro)

public:
    Graph() { n = 0; }

    // Đọc dữ liệu từ file
    bool readFromFile(const string &filename) {
        ifstream f(filename);
        if (!f.is_open()) {
            cout << "Khong the mo file: " << filename << "\n";
            return false;
        }

        int m;
        f >> n >> m;
        
        // Kiểm tra n, m hợp lệ
        if (n <= 0 || m < 0) {
            cout << "Du lieu trong file khong hop le!\n";
            return false;
        }

        w.assign(n, vector<int>(n, 0));
        adj.assign(n, vector<pair<int,int>>());

        for (int i = 0; i < m; i++) {
            int u, v, risk;
            f >> u >> v >> risk;
            
            // Chuyển từ 1-indexed sang 0-indexed
            u--; v--;
            
            // Kiểm tra u, v nằm trong [0, n-1]
            if (u < 0 || u >= n || v < 0 || v >= n) {
                cout << "Chi so dinh khong hop le: " << (u+1) << " " << (v+1) << "\n";
                continue;
            }
            
            w[u][v] = w[v][u] = risk;
            adj[u].push_back({v, risk});
            adj[v].push_back({u, risk});
        }

        f.close();
        cout << "Doc file thanh cong! (n=" << n << ", m=" << m << ")\n";
        return true;
    }

    // In ma trận rủi ro
    void printMatrix() {
        if (n == 0) { cout << "Chua co du lieu!\n"; return; }

        cout << "\n=== MA TRAN RUI RO TAI CHINH ===\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                cout << w[i][j] << " ";
            cout << "\n";
        }
    }

    // In danh sách kề
    void printAdjList() {
        if (n == 0) { cout << "Chua co du lieu!\n"; return; }

        cout << "\n=== DANH SACH KE (Quan he rui ro) ===\n";
        for (int u = 0; u < n; u++) {
            cout << "Tai san " << u << ": ";
            for (auto &p : adj[u])
                cout << "(" << p.first << ", r=" << p.second << ") ";
            cout << "\n";
        }
    }

    // BFS kiểm tra liên thông
    bool isConnected() {
        if (n == 0) return false;

        vector<bool> visited(n, false);
        queue<int> q;
        q.push(0);
        visited[0] = true;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto &p : adj[u]) {
                int v = p.first;
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }

        return all_of(visited.begin(), visited.end(), [](bool x){ return x; });
    }

    // Thuật toán Prim O(n^2)
    int primMatrix() {
        if (n == 0) {
            cout << "Chua co du lieu!\n";
            return 0;
        }
if (!isConnected()) {
            cout << "Do thi khong lien thong, khong co MST!\n";
            return 0;
        }

        vector<bool> used(n, false);
        vector<int> min_risk(n, INT_MAX);
        min_risk[0] = 0;

        int total = 0;

        for (int it = 0; it < n; it++) {
            int u = -1;
            for (int i = 0; i < n; i++) {
                if (!used[i] && (u == -1 || min_risk[i] < min_risk[u]))
                    u = i;
            }

            used[u] = true;
            total += min_risk[u];

            for (int v = 0; v < n; v++) {
                if (w[u][v] != 0 && !used[v] && w[u][v] < min_risk[v])
                    min_risk[v] = w[u][v];
            }
        }
        return total;
    }
};

// In menu chủ đề tài chính (Prim)
void printMenu() {
    cout << "\n====== PHAN TICH RUI RO TAI CHINH - THUAT TOAN PRIM ======\n";
    cout << "1. Doc mang rui ro tai chinh tu file\n";
    cout << "2. In ma tran rui ro\n";
    cout << "3. In danh sach ke (cac quan he rui ro)\n";
    cout << "4. Kiem tra su ket noi cua he thong tai chinh\n";
    cout << "5. Phan tich rui ro bang Prim (O(n^2))\n";
    cout << "6. Thoat\n";
    cout << "Chon: ";
}

int main() {
    Graph g;
    int choice;

    while (true) {
        printMenu();
        cin >> choice;

        if (choice == 1) {
            string file;
            cout << "Nhap ten file: ";
            cin >> file;
            g.readFromFile(file);
        }
        else if (choice == 2) g.printMatrix();
        else if (choice == 3) g.printAdjList();
        else if (choice == 4) {
            if (g.isConnected()) cout << "He thong tai chinh lien thong!\n";
            else cout << "He thong khong lien thong!\n";
        }
        else if (choice == 5) {
            cout << "Tong muc rui ro toi thieu (MST) = " << g.primMatrix() << "\n";
        }
        else if (choice == 6) break;
        else cout << "Lua chon khong hop le!\n";
    }

    return 0;
}