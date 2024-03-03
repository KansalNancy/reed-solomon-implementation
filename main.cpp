#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

#define W 4
#define NW (1 << W)

vector<int> gflog;
vector<int> gfilog;

void log_table()
{
    int length_of_log_table = pow(2, W) - 1;
    gflog.resize(length_of_log_table + 1, 0);
    gfilog.resize(length_of_log_table + 1, 0);

    gflog[0] = -1;
    gfilog[0] = 1;
    gflog[1] = 0;

    for (int i = 1; i <= length_of_log_table; i++)
    {
        int x = gfilog[i - 1] << 1;

        // Perform XOR with binary value "10011" (decimal 19) when necessary
        if (x >= (1 << W))
        {
            int binaryDecimal = 19;
            x ^= binaryDecimal;
        }

        gfilog[i] = x;
        gflog[x] = i;
    }

    gflog[1] = 0;
    gfilog[length_of_log_table] = -1;
}

int mult(int a, int b)
{
    int sum_log;
    if (a == 0 || b == 0)
        return 0;
    sum_log = gflog[a] + gflog[b];
    if (sum_log >= NW - 1)
        sum_log -= NW - 1;
    return gfilog[sum_log];
}

int divide(int a, int b)
{
    int diff_log;
    if (a == 0)
        return 0;
    if (b == 0)
        return -1;
    diff_log = gflog[a] - gflog[b];
    if (diff_log < 0)
        diff_log += (1 << W) - 1;
    return gfilog[diff_log];
}

int XOR(int a, int b)
{
    return a ^ b;
}

vector<vector<int>> generate_f(int m, int n)
{
    vector<vector<int>> matrix_temp_f(m, vector<int>(n, 1));

    for (int i = 1; i < m; i++)
    {
        for (int j = 1; j < n; j++)
        {
            int x, y;
            if (i != m && j != n)
            {
                x = i + 1;
                y = j + 1;
            }
            else
            {
                x = m;
                y = n;
            }
            int answer = 1;
            for (int k = 1; k <= x - 1; k++)
            {
                answer = mult(y, answer);
            }
            matrix_temp_f[i][j] = answer;
        }
    }
    return matrix_temp_f;
}

vector<vector<int>> generate_check_sum(vector<vector<int>> f, vector<vector<int>> d)
{
    int f_m = f.size();
    int d_m = d.size();
    int d_n = d[0].size();
    vector<vector<int>> matrix_temp(f_m, vector<int>(d_n, 0));
    for (int i = 0; i < f_m; i++)
    {
        for (int j = 0; j < d_n; j++)
        {
            for (int k = 0; k < d_m; k++)
            {
                matrix_temp[i][j] = XOR(matrix_temp[i][j], mult(f[i][k], d[k][j]));
            }
        }
    }
    return matrix_temp;
}

vector<vector<int>> generate_a(vector<vector<int>> f)
{
    vector<vector<int>> matrix_temp(f.size() + f.size(), vector<int>(f[0].size(), 0));
    int row = f.size();
    int col = f[0].size();
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            matrix_temp[i][j] = (i == j) ? 1 : 0;
        }
    }

    for (int i = f.size(); i < f.size() + f.size(); i++)
    {
        for (int j = 0; j < col; j++)
        {
            matrix_temp[i][j] = f[i - f.size()][j];
        }
    }
    return matrix_temp;
}

vector<vector<int>> generate_e(vector<vector<int>> d, vector<vector<int>> c)
{
    vector<vector<int>> matrix_temp(d.size() + c.size(), vector<int>(d[0].size(), 0));
    int row = d.size() + c.size();
    int col = d[0].size();
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            matrix_temp[i][j] = (i < d.size()) ? d[i][j] : c[i - d.size()][j];
        }
    }
    return matrix_temp;
}

int main()
{
    log_table();

    vector<vector<int>> D_for_data(3, vector<int>(1, 0));
    D_for_data[0][0] = 3;
    D_for_data[1][0] = 13;
    D_for_data[2][0] = 9;

    vector<vector<int>> F_for_f = generate_f(3, 3);

    vector<vector<int>> check_sum = generate_check_sum(F_for_f, D_for_data);

    vector<vector<int>> matrix_A = generate_a(F_for_f);
    for (int i = 0; i < matrix_A.size(); i++)
    {
        for (int j = 0; j < matrix_A[0].size(); j++)
        {
            cout << matrix_A[i][j] << " ";
        }
        cout << endl;
    }
    cout << "------------------------" << endl;

    vector<vector<int>> matrix_E = generate_e(D_for_data, check_sum);
    for (int i = 0; i < matrix_E.size(); i++)
    {
        for (int j = 0; j < matrix_E[0].size(); j++)
        {
            cout << matrix_E[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}
