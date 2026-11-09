#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura para armazenar os dados do investidor
typedef struct {
    char nome[100];
    char cpf[15];
    char senha[10];
    double reais;
    double bitcoin;
    double ethereum;
    double ripple;
} Investidor;

// Estrutura para armazenar as cotacoes atuais
typedef struct {
    double bitcoin;
    double ethereum;
    double ripple;
} Cotacoes;

// Protótipos das funções
void carregarCotacoes(Cotacoes *cot);
void salvarCotacoes(Cotacoes cot);
int efetuarLogin(Investidor *inv);
void salvarInvestidor(Investidor inv);
void registrarExtrato(const char *cpf, const char *descricao);
void consultarSaldo(Investidor inv);
void consultarExtrato(Investidor inv);
void depositar(Investidor *inv);
void sacar(Investidor *inv);
void comprarCripto(Investidor *inv, Cotacoes cot);
void venderCripto(Investidor *inv, Cotacoes cot);
void atualizarCotacao(Cotacoes *cot);

// Carrega as cotações de um arquivo texto ou define padrões caso não exista
void carregarCotacoes(Cotacoes *cot) {
    FILE *f = fopen("cotacoes.txt", "r");
    if (f == NULL) {
        // Valores iniciais padrão caso o arquivo não exista
        cot->bitcoin = 300000.0;
        cot->ethereum = 15000.0;
        cot->ripple = 3.0;
        salvarCotacoes(*cot);
    } else {
        fscanf(f, "%lf %lf %lf", &cot->bitcoin, &cot->ethereum, &cot->ripple);
        fclose(f);
    }
}

// Salva as cotações no arquivo
void salvarCotacoes(Cotacoes cot) {
    FILE *f = fopen("cotacoes.txt", "w");
    if (f != NULL) {
        fprintf(f, "%.2f %.2f %.2f\n", cot.bitcoin, cot.ethereum, cot.ripple);
        fclose(f);
    }
}

// Salva as informações do investidor no arquivo "investidor.txt"
void salvarInvestidor(Investidor inv) {
    FILE *f = fopen("investidor.txt", "w");
    if (f != NULL) {
        fprintf(f, "%s\n%s\n%s\n%.2f\n%.6f\n%.6f\n%.6f\n",
                inv.nome, inv.cpf, inv.senha, inv.reais, inv.bitcoin, inv.ethereum, inv.ripple);
        fclose(f);
    }
}
// Efetua login verificando CPF e senha gravados em arquivo
int efetuarLogin(Investidor *inv) {
    char cpfEntrada[15], senhaEntrada[10];

    FILE *f = fopen("investidor.txt", "r");
    if (f == NULL) {
        // Cria um usuario padrao de teste se o arquivo nao existir
        strcpy(inv->nome, "Gabriela Biondi");
        strcpy(inv->cpf, "12345678900");
        strcpy(inv->senha, "123456");
        inv->reais = 580.00;
        inv->bitcoin = 0.0158;
        inv->ethereum = 1.03;
        inv->ripple = 200.35;
        salvarInvestidor(*inv);
    } else {
        fgets(inv->nome, sizeof(inv->nome), f);
        inv->nome[strcspn(inv->nome, "\n")] = 0; // Remove a quebra de linha
        fscanf(f, "%s\n%s\n%lf\n%lf\n%lf\n%lf", 
               inv->cpf, inv->senha, &inv->reais, &inv->bitcoin, &inv->ethereum, &inv->ripple);
        fclose(f);
    }

    printf("\n--- LOGIN DE INVESTIDOR ---\n");
    printf("Digite seu CPF (apenas numeros): ");
    scanf("%s", cpfEntrada);
    printf("Digite sua senha de 6 digitos: ");
    scanf("%s", senhaEntrada);

    if (strcmp(cpfEntrada, inv->cpf) == 0 && strcmp(senhaEntrada, inv->senha) == 0) {
        printf("\nLogin realizado com sucesso! Bem-vindo(a), %s.\n", inv->nome);
        return 1;
    } else {
        printf("\nCPF ou senha incorretos.\n");
        return 0;
    }
}

// Registra uma operação no extrato texto com data/hora
void registrarExtrato(const char *cpf, const char *descricao) {
    char nomeArquivo[30];
    sprintf(nomeArquivo, "extrato_%s.txt", cpf);

    FILE *f = fopen(nomeArquivo, "a");
    if (f != NULL) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        
        fprintf(f, "%02d-%02d-%04d %02d:%02d - %s\n", 
                tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, 
                tm.tm_hour, tm.tm_min, descricao);
        fclose(f);
    }
}

// Requisito 1: Consultar Saldo
void consultarSaldo(Investidor inv) {
    char senha[10];
    printf("\nDigite sua senha para confirmar: ");
    scanf("%s", senha);

    if (strcmp(senha, inv.senha) != 0) {
        printf("Senha incorreta!\n");
        return;
    }

    printf("\n=========================================\n");
    printf("Nome: %s\n", inv.nome);
    printf("CPF: %s\n", inv.cpf);
    printf("Reais: R$ %.2f\n", inv.reais);
    printf("Bitcoin: %.6f\n", inv.bitcoin);
    printf("Ethereum: %.6f\n", inv.ethereum);
    printf("Ripple: %.6f\n", inv.ripple);
    printf("=========================================\n");
}

// Requisito 2: Consultar Extrato
void consultarExtrato(Investidor inv) {
    char senha[10];
    printf("\nDigite sua senha para confirmar: ");
    scanf("%s", senha);

    if (strcmp(senha, inv.senha) != 0) {
        printf("Senha incorreta!\n");
        return;
    }

    char nomeArquivo[30];
    sprintf(nomeArquivo, "extrato_%s.txt", inv.cpf);

    FILE *f = fopen(nomeArquivo, "r");
    printf("\n================ EXTRATO ================\n");
    printf("Nome: %s\n", inv.nome);
    printf("CPF: %s\n", inv.cpf);
    printf("-----------------------------------------\n");

    if (f == NULL) {
        printf("Nenhuma transacao registrada ainda.\n");
    } else {
        char linha[200];
        while (fgets(linha, sizeof(linha), f) != NULL) {
            printf("%s", linha);
        }
        fclose(f);
    }
    printf("=========================================\n");
}
