#!/bin/bash

# Исполняемый файл (первый аргумент, если не задан — ./main)
PROG="${1:-./main}"

# Папки для тестовых данных
TESTS_DIR="tests"
OUTPUT_DIR="test_outputs"

mkdir -p "$TESTS_DIR" "$OUTPUT_DIR"

passed=0
failed=0

run_test() {
    local name="$1"
    local input="$2"
    local expected="$3"
    local output="$OUTPUT_DIR/$name.out"

    echo -n "Test $name: "
    "$PROG" "$input" > "$output" 2>&1
    if diff -q "$output" "$expected" > /dev/null 2>&1; then
        echo "PASS"
        ((passed++))
    else
        echo "FAIL"
        echo "  Expected:"
        cat "$expected"
        echo "  Got:"
        cat "$output"
        ((failed++))
    fi
}

# Тест 1: один город, одна столица
cat > "$TESTS_DIR/test1.in" <<EOF
1 0
1
1
EOF
cat > "$TESTS_DIR/test1.expected" <<EOF
Capital 1 has cities:
EOF

# Тест 2: два города, одна дорога, две столицы
cat > "$TESTS_DIR/test2.in" <<EOF
2 1
1 2 5
2
1 2
EOF
cat > "$TESTS_DIR/test2.expected" <<EOF
Capital 1 has cities:
Capital 2 has cities:
EOF

# Тест 3: два города, одна дорога, одна столица
cat > "$TESTS_DIR/test3.in" <<EOF
2 1
1 2 3
1
1
EOF
cat > "$TESTS_DIR/test3.expected" <<EOF
Capital 1 has cities: 2
EOF

# Тест 4: три города в линию, столицы 1 и 3 (равные расстояния)
cat > "$TESTS_DIR/test4.in" <<EOF
3 2
1 2 10
2 3 10
2
1 3
EOF
cat > "$TESTS_DIR/test4.expected" <<EOF
Capital 1 has cities: 2
Capital 3 has cities:
EOF

# Тест 5
cat > "$TESTS_DIR/test5.in" <<EOF
4 3
1 3 8
2 4 9
3 4 11
2
2 3
EOF
cat > "$TESTS_DIR/test5.expected" <<EOF
Capital 2 has cities: 4
Capital 3 has cities: 1
EOF

# Тест 6: 5 городов
cat > "$TESTS_DIR/test6.in" <<EOF
5 6
1 2 2
1 3 5
2 3 1
2 4 4
3 5 6
4 5 3
2
1 5
EOF
cat > "$TESTS_DIR/test6.expected" <<EOF
Capital 1 has cities: 2 3
Capital 5 has cities: 4
EOF

# Тест 7: все города — столицы
cat > "$TESTS_DIR/test7.in" <<EOF
3 2
1 2 10
2 3 20
3
1 2 3
EOF
cat > "$TESTS_DIR/test7.expected" <<EOF
Capital 1 has cities:
Capital 2 has cities:
Capital 3 has cities:
EOF

run_test "test1" "$TESTS_DIR/test1.in" "$TESTS_DIR/test1.expected"
run_test "test2" "$TESTS_DIR/test2.in" "$TESTS_DIR/test2.expected"
run_test "test3" "$TESTS_DIR/test3.in" "$TESTS_DIR/test3.expected"
run_test "test4" "$TESTS_DIR/test4.in" "$TESTS_DIR/test4.expected"
run_test "test5" "$TESTS_DIR/test5.in" "$TESTS_DIR/test5.expected"
run_test "test6" "$TESTS_DIR/test6.in" "$TESTS_DIR/test6.expected"
run_test "test7" "$TESTS_DIR/test7.in" "$TESTS_DIR/test7.expected"

echo "Passed: $passed, Failed: $failed"
exit $failed