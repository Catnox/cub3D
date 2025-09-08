#!/bin/bash

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_cub3d.sh                                      :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/31 15:30:00 by radubos           #+#    #+#              #
#    Updated: 2025/08/31 15:30:00 by radubos          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

echo "🎮 Testing cub3D..."

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test compilation
echo -e "${YELLOW}📦 Testing compilation...${NC}"
make re
if [ $? -ne 0 ]; then
    echo -e "${RED}❌ Compilation failed${NC}"
    exit 1
fi
echo -e "${GREEN}✅ Compilation successful${NC}"

# Test valid maps
echo -e "${YELLOW}🗺️  Testing valid maps...${NC}"
for map in maps/*.cub; do
    if [ -f "$map" ]; then
        echo "Testing $map..."
        timeout 2s ./cub3D "$map" &> /dev/null
        if [ $? -eq 0 ] || [ $? -eq 124 ]; then
            echo -e "${GREEN}✅ $map: OK${NC}"
        else
            echo -e "${RED}❌ $map: FAILED${NC}"
        fi
    fi
done

# Test memory leaks
echo -e "${YELLOW}🧠 Testing memory leaks...${NC}"
if command -v valgrind &> /dev/null; then
    valgrind --leak-check=full --show-leak-kinds=all ./cub3D maps/test_simple.cub 2>&1 | grep "no leaks are possible"
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✅ No memory leaks detected${NC}"
    else
        echo -e "${RED}❌ Memory leaks detected${NC}"
    fi
else
    echo -e "${YELLOW}⚠️  Valgrind not available (install with: brew install valgrind)${NC}"
fi

echo -e "${GREEN}🏁 Testing complete${NC}"
