#include <cstring>
#include <iomanip>

#include "number.h"

uint2022_t from_uint(uint32_t i) {
    uint2022_t num;
    uint16_t iter = 0;
    while (i > 0) {
        num.digits[iter++] = (i % uint2022_t::BASE);
        i /= uint2022_t::BASE;
    }

    return num;
}

uint2022_t from_string(const char* buff) {
    uint2022_t num;
    const int digit_count = 9;
    char res[digit_count];
    for (int k = 0; k<digit_count; k++){
        res[k] = '0';
    }
    uint16_t iter = 0;
    int16_t i = strlen(buff)-1;
    int j = digit_count-1;

    while (i>=0) {
        res[j--] = buff[i--];
        if (j == -1 || i==-1) {
            num.digits[iter++] = (atoi(res));
            for (int k = 0; k<digit_count; k++){
                res[k] = '0';
            }
            j = digit_count-1;
        }
    }

    return num;
}

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs) {
    for (int16_t i = 0; i < uint2022_t::SIZE; i++) {
        if (lhs.digits[i] != rhs.digits[i]) {
            return false;
        }
    }

    return true;
}

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs) {
    return !(lhs == rhs);
}

bool operator<(const uint2022_t& lhs, const uint2022_t& rhs) {
    for (int16_t i = uint2022_t::SIZE - 1; i >= 0; --i) {
        if (lhs.digits[i] != rhs.digits[i]) {
            return lhs.digits[i] < rhs.digits[i];
        }
    }

    return false;
}

bool operator>(const uint2022_t& lhs, const uint2022_t& rhs) {
    return (rhs < lhs);
}

bool operator<=(const uint2022_t& lhs, const uint2022_t& rhs) {
    return !(lhs > rhs);
}

bool operator>=(const uint2022_t& lhs, const uint2022_t& rhs) {
    return !(lhs < rhs);
}

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t num;
    uint16_t iter = 0;
    uint32_t overload = 0;
    for (int16_t i = 0; i < uint2022_t::SIZE || overload; i++) {
        uint32_t sum = (lhs.digits[i] + rhs.digits[i] + overload) % uint2022_t::BASE;
        overload = (lhs.digits[i] + rhs.digits[i] + overload) / uint2022_t::BASE;
        num.digits[iter++] = (sum);
    }

    return num;
}

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t num;
    for (uint16_t i = 0; i < uint2022_t::SIZE; i++) {
        num.digits[i] = lhs.digits[i];
    }

    uint8_t underload = 0;
    for (int16_t i = 0; i < uint2022_t::SIZE; i++) {
        int64_t value = (int64_t) num.digits[i] - (underload + (int64_t) rhs.digits[i]);
        underload = value < 0;
        if (underload) {
            value += uint2022_t::BASE;
        }
        num.digits[i] = value;
    }

    return num;
}

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t num;
    for (int16_t i = 0; i < uint2022_t::SIZE; i++) {
        uint64_t overload = 0;
        for (int16_t j = 0; j < uint2022_t::SIZE - i || overload; j++) {
            uint64_t cur = num.digits[i + j] + lhs.digits[i] * 1ll * rhs.digits[j] + overload;
            num.digits[i + j] = cur % uint2022_t::BASE;
            overload = cur / uint2022_t::BASE;
        }
    }

    return num;
}

uint2022_t shift_right(uint2022_t& num) {
    for (int16_t i = uint2022_t::SIZE - 1; i > 0; i--) {
        num.digits[i] = num.digits[i - 1];
    }
    num.digits[0] = 0;

    return num;
}

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs) {
    if (rhs == from_uint(0)) {
        std::cout << "Division by zero! ";

        return from_uint(0);
    }

    uint2022_t result;
    uint2022_t current;

    for (int16_t i = uint2022_t::SIZE - 1; i >= 0; i--) {
        current = shift_right(current);
        current.digits[0] = lhs.digits[i];

        int32_t x = 0;
        int32_t l = 0;
        int32_t r = uint2022_t::BASE;
        while (l <= r) {
            int32_t m = (l + r) / 2;
            uint2022_t t = rhs * from_uint(m);
            if (t < current || t == current) {
                x = m;
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
        result.digits[i] = x;
        current = current - rhs * from_uint(x);
    }

    return result;
}


std::ostream& operator<<(std::ostream& stream, const uint2022_t& value) {
    int16_t iter = uint2022_t::SIZE;
    while (value.digits[--iter] == 0);

    if (iter < 0) {
        stream << 0;
        return stream;
    }

    stream << value.digits[iter--];
    while (iter >= 0) {
        stream << std::setw(9) << std::setfill('0') << value.digits[iter--];
    }

    return stream;
}
