#!/bin/bash

# Lista de todos os arquivos .cpp do projeto
ALL_CPP=$(find src libs -name "*.cpp")
TOTAL=$(echo "$ALL_CPP" | wc -l)

echo "🔹 Total de arquivos a compilar: $TOTAL"

# Tempo de início
START_TIME=$(date +%s)
COMPILED=0

# Executa make e processa saída
make -j$(nproc) 2>&1 | while read -r line; do
    # Detecta compilação de um arquivo
    if [[ "$line" =~ \.cpp ]]; then
        FILE=$(echo "$line" | grep -oP '[^ ]+\.cpp')
        ((COMPILED++))

        # Calcula tempo decorrido e estimativa
        NOW=$(date +%s)
        ELAPSED=$((NOW - START_TIME))
        if (( COMPILED > 0 )); then
            AVG_TIME=$(echo "$ELAPSED / $COMPILED" | bc -l)
            REMAINING=$((TOTAL - COMPILED))
            ETA=$(echo "$AVG_TIME * $REMAINING" | bc -l | cut -d. -f1)
        else
            ETA=0
        fi

        echo "📄 [$COMPILED/$TOTAL] $FILE | ⏳ ETA: ${ETA}s"
    else
        # Passa qualquer outra saída do make sem alteração
        echo "$line"
    fi
done
