#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

class Print {
public:
    enum Align { LEFT, RIGHT, CENTER };

    // 设置表头
    template<typename... Args>
    void setHeaders(Args&&... args) {
        headers = toStringVector(std::forward<Args>(args)...);
    }
    const std::vector<std::string>& getHeaders() const {
        return headers;
    }

    // 添加行
    template<typename... Args>
    void addRow(Args&&... args) {
        rows.emplace_back(toStringVector(std::forward<Args>(args)...));
    }

    // 设置每列对齐方式
    void setAlignments(const std::vector<Align>& aligns) {
        alignments = aligns;
    }

    // 设置每列最大宽度（超出则截断）
    void setMaxWidths(const std::vector<size_t>& widths) {
        maxWidths = widths;
    }

    void print() const {
        if (headers.empty()) {
            std::cerr << "No header set.\n";
            return;
        }

        auto colWidths = getColumnWidths();

        printSeparator(colWidths);
        printRow(headers, colWidths);
        printSeparator(colWidths);

        for (const auto& row : rows) {
            printRow(row, colWidths);
        }

        printSeparator(colWidths);
    }

private:
    std::vector<std::string> headers;
    std::vector<std::vector<std::string>> rows;
    std::vector<Align> alignments;
    std::vector<size_t> maxWidths;

    std::vector<size_t> getColumnWidths() const {
        std::vector<size_t> colWidths(headers.size(), 0);

        for (size_t i = 0; i < headers.size(); ++i) {
            colWidths[i] = applyWidthLimit(headers[i], i).length();
        }

        for (const auto& row : rows) {
            for (size_t i = 0; i < row.size(); ++i) {
                std::string cell = applyWidthLimit(row[i], i);
                if (i < colWidths.size()) {
                    colWidths[i] = std::max(colWidths[i], cell.length());
                }
            }
        }

        return colWidths;
    }

    void printSeparator(const std::vector<size_t>& colWidths) const {
        std::cout << "+";
        for (auto w : colWidths) {
            std::cout << std::string(w + 2, '-') << "+";
        }
        std::cout << std::endl;
    }

    void printRow(const std::vector<std::string>& row, const std::vector<size_t>& colWidths) const {
        std::cout << "|";
        for (size_t i = 0; i < colWidths.size(); ++i) {
            std::string cell = (i < row.size()) ? applyWidthLimit(row[i], i) : "";
            size_t width = colWidths[i];
            Align align = (i < alignments.size()) ? alignments[i] : LEFT;

            std::string formatted;
            if (align == LEFT) {
                formatted = cell + std::string(width - cell.length(), ' ');
            } else if (align == RIGHT) {
                formatted = std::string(width - cell.length(), ' ') + cell;
            } else { // CENTER
                size_t pad = width - cell.length();
                size_t left = pad / 2;
                size_t right = pad - left;
                formatted = std::string(left, ' ') + cell + std::string(right, ' ');
            }

            std::cout << " " << formatted << " |";
        }
        std::cout << std::endl;
    }

    std::string applyWidthLimit(const std::string& s, size_t index) const {
        if (index >= maxWidths.size() || s.length() <= maxWidths[index]) {
            return s;
        }

        size_t maxW = maxWidths[index];
        if (maxW <= 3) return s.substr(0, maxW);  // not enough space for ellipsis
        return s.substr(0, maxW - 3) + "...";
    }

    template<typename T>
    std::string toString(const T& val) const {
        std::ostringstream oss;
        oss << val;
        return oss.str();
    }

    template<typename... Args>
    std::vector<std::string> toStringVector(Args&&... args) const {
        return { toString(std::forward<Args>(args))... };
    }
};
