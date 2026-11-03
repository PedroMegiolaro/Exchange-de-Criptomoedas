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
