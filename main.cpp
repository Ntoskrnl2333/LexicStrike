#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>

using namespace std;

// 存储单词及其释义
struct Word {
    string word;
    vector<string> meanings;
};

// 随机数生成器
mt19937 rng((unsigned)time(nullptr));

int main() {
    ifstream file("words.csv");
    if (!file.is_open()) {
        cerr << "无法打开 words.csv 文件" << endl;
        return 1;
    }

    vector<Word> words;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string item;
        Word w;

        // 先读取单词
        if (getline(ss, item, ',')) {
            w.word = item;
        }
        // 再读取释义
        while (getline(ss, item, ',')) {
            if (!item.empty())
                w.meanings.push_back(item);
        }

        if (!w.word.empty() && !w.meanings.empty()) {
            words.push_back(w);
        }
    }
    file.close();

    if (words.empty()) {
        cerr << "单词列表为空" << endl;
        return 1;
    }

    // 打乱顺序，避免重复
    vector<int> indices(words.size());
    iota(indices.begin(), indices.end(), 0);
    shuffle(indices.begin(), indices.end(), rng);

    int correctCount = 0; // 正确数
    int totalCount = 0;   // 总题数

    for (int idx : indices) {
        Word &w = words[idx];

        // 正确答案
        string correct = w.meanings[rng() % w.meanings.size()];

        // 生成选项
        vector<string> options;
        options.push_back(correct);

        // 干扰项最多 5 个
        while (options.size() < 6 && options.size() < words.size()) {
            int otherIdx = rng() % words.size();
            if (otherIdx == idx) continue;
            Word &other = words[otherIdx];
            if (other.meanings.empty()) continue;
            string fake = other.meanings[rng() % other.meanings.size()];
            if (find(options.begin(), options.end(), fake) == options.end()) {
                options.push_back(fake);
            }
        }

        // 打乱选项
        shuffle(options.begin(), options.end(), rng);

        // 输出题目
        cout << "请翻译单词: " << w.word << endl;
        for (size_t i = 0; i < options.size(); i++) {
            cout << i + 1 << ". " << options[i] << endl;
        }

    INVALID_INPUT:

        cout << "请输入选项编号 (0退出): ";
        int choice;
        cin >> choice;
        if (choice == 0) break;

        if (choice < 1 || choice > (int)options.size()) {
            cout << "无效输入" << endl;
            goto INVALID_INPUT;
        }

        totalCount++;
        if (options[choice - 1] == correct) {
            cout << "✔ 正确!" << endl;
            correctCount++;
        } else {
            cout << "✘ 错误! 正确答案是: " << correct << endl;
        }

        // 实时显示成绩
        cout << "当前得分: " << correctCount << " / " << totalCount
             << "  (正确率 " << (100.0 * correctCount / totalCount) << "%)" << endl;

        cout << "----------------------" << endl;
    }

    // 退出时总结
    cout << "测验结束！总成绩: " << correctCount << " / " << totalCount;
    if (totalCount > 0) {
        cout << "  (正确率 " << (100.0 * correctCount / totalCount) << "%)";
    }
    cout << endl;

    return 0;
}
