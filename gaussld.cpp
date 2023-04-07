ld EPS = 1e-9;
vector<int> where;
int solve_system(vector<vector<ld>> a, vector<ld> &b){
    int n = a.size();
    int m = a[0].size();
    for (int i = 0; i < n; ++i) a[i].push_back(b[i]); 

    where.assign(m, -1);
    for (int col = 0, row = 0; col < m && row < n; ++col){
        int sel = row;
        for (int i = row; i < n; ++i) if (abs(a[i][col]) > abs(a[sel][col])) sel = i;
        if (abs(a[sel][col]) < EPS) continue;
        swap(a[sel], a[row]);
        where[col] = row;

        for (int i = 0; i < n; ++i) if (i != row){
            ld c = a[i][col] / a[row][col];
            for (int j = 0; j <= m; ++j) a[i][j] -= a[row][j]*c;
        } 

        ++row;
    }


    b.assign(m, 0);
    for (int i = 0; i < m; ++i) if (where[i] != -1) b[i] = a[where[i]][m] / a[where[i]][i];

    // test if inconsistent solution
    for (int i = 0; i < n; ++i) {
        ld sum = 0;
        for (int j = 0; j < m; ++j) sum += b[j] * a[i][j];
        if (abs(sum - a[i][m]) > EPS) return 0;
    }

    // infinite solutions case
    for (int i = 0; i < m; ++i) if (where[i] == -1) return 2;

    return 1;
}

