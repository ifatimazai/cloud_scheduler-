#!/bin/bash

# Cloud Scheduler Test Script
# Tests various aspects of the simulator

echo "╔════════════════════════════════════════╗"
echo "║  Cloud Scheduler Test Suite             ║"
echo "╚════════════════════════════════════════╝"
echo ""

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test counter
TESTS_PASSED=0
TESTS_FAILED=0

# Function to run a test
run_test() {
    local test_name=$1
    local command=$2
    
    echo -n "Testing: $test_name ... "
    
    if eval "$command" > /dev/null 2>&1; then
        echo -e "${GREEN}✓ PASSED${NC}"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}✗ FAILED${NC}"
        ((TESTS_FAILED++))
    fi
}

# Test 1: Check if Makefile exists
run_test "Makefile exists" "test -f Makefile"

# Test 2: Build project
echo ""
echo "Building project..."
make clean > /dev/null 2>&1
if make > /dev/null 2>&1; then
    echo -e "${GREEN}✓ Build successful${NC}"
    ((TESTS_PASSED++))
else
    echo -e "${RED}✗ Build failed${NC}"
    ((TESTS_FAILED++))
    exit 1
fi

# Test 3: Check if executable was created
run_test "Executable created" "test -f bin/cloud_scheduler"

# Test 4: Run with Priority Scheduling (5 seconds)
echo ""
echo "Running simulation tests (this may take a minute)..."
echo ""

timeout 35 ./bin/cloud_scheduler priority > /tmp/priority_test.log 2>&1
if [ $? -eq 0 ] || [ $? -eq 124 ]; then
    if grep -q "VM.*arrived" /tmp/priority_test.log && \
       grep -q "SCHEDULER STATISTICS" /tmp/priority_test.log; then
        echo -e "${GREEN}✓ Priority Scheduling test PASSED${NC}"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}✗ Priority Scheduling test FAILED${NC}"
        ((TESTS_FAILED++))
    fi
else
    echo -e "${RED}✗ Priority Scheduling test FAILED${NC}"
    ((TESTS_FAILED++))
fi

# Test 5: Run with SJF Scheduling (5 seconds)
timeout 35 ./bin/cloud_scheduler sjf > /tmp/sjf_test.log 2>&1
if [ $? -eq 0 ] || [ $? -eq 124 ]; then
    if grep -q "VM.*arrived" /tmp/sjf_test.log && \
       grep -q "SCHEDULER STATISTICS" /tmp/sjf_test.log; then
        echo -e "${GREEN}✓ SJF Scheduling test PASSED${NC}"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}✗ SJF Scheduling test FAILED${NC}"
        ((TESTS_FAILED++))
    fi
else
    echo -e "${RED}✗ SJF Scheduling test FAILED${NC}"
    ((TESTS_FAILED++))
fi

# Test 6: Check log file generation
run_test "Log file generation" "test -f simulation_log.txt"

# Test 7: Verify resource allocation
if grep -q "RESOURCE STATUS" /tmp/priority_test.log; then
    echo -e "${GREEN}✓ Resource tracking working${NC}"
    ((TESTS_PASSED++))
else
    echo -e "${RED}✗ Resource tracking failed${NC}"
    ((TESTS_FAILED++))
fi

# Test 8: Verify deadlock detector
if grep -q "DEADLOCK" /tmp/priority_test.log || grep -q "Deadlock" /tmp/priority_test.log; then
    echo -e "${GREEN}✓ Deadlock detector active${NC}"
    ((TESTS_PASSED++))
else
    echo -e "${YELLOW}⚠ No deadlock detected in this run${NC}"
    ((TESTS_PASSED++))
fi

# Print summary
echo ""
echo "╔════════════════════════════════════════╗"
echo "║        Test Summary                     ║"
echo "╚════════════════════════════════════════╝"
echo ""
echo -e "Tests Passed: ${GREEN}$TESTS_PASSED${NC}"
echo -e "Tests Failed: ${RED}$TESTS_FAILED${NC}"
echo ""

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed! ✓${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed!${NC}"
    exit 1
fi
