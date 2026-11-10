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

int main() {
    Investidor usuario;
    Cotacoes cotacoes;

    // Carrega as cotações salvas em arquivo
    carregarCotacoes(&cotacoes);

    printf("=========================================\n");
    printf("   BEM-VINDO A EXCHANGE DE CRIPTOMOEDAS  \n");
    printf("=========================================\n");

    // Tela de Login[cite: 6, 7]
    if (!efetuarLogin(&usuario)) {
        printf("\nLogin falhou! Encerrando o programa...\n");
        return 0;
    }

    int opcao = 0;
    do {
        printf("\n-----------------------------------------\n");
        printf("               MENU PRINCIPAL             \n");
        printf("-----------------------------------------\n");
        printf("1. Consultar saldo\n");
        printf("2. Consultar extrato\n");
        printf("3. Depositar reais\n");
        printf("4. Sacar reais\n");
        printf("5. Comprar criptomoedas\n");
        printf("6. Vender criptomoedas\n");
        printf("7. Atualizar cotacao\n");
        printf("8. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                consultarSaldo(usuario);
                break;
            case 2:
                consultarExtrato(usuario);
                break;
            case 3:
                depositar(&usuario);
                break;
            case 4:
                sacar(&usuario);
                break;
            case 5:
                comprarCripto(&usuario, cotacoes);
                break;
            case 6:
                venderCripto(&usuario, cotacoes);
                break;
            case 7:
                atualizarCotacao(&cotacoes);
                break;
            case 8:
                printf("\nSaindo... Obrigado por utilizar nossa Exchange!\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 8);

    return 0;
}

// Carrega as cotações de um arquivo texto ou define padrões caso não exista[cite: 4]
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

// Salva as cotações no arquivo[cite: 4]
void salvarCotacoes(Cotacoes cot) {
    FILE *f = fopen("cotacoes.txt", "w");
    if (f != NULL) {
        fprintf(f, "%.2f %.2f %.2f\n", cot.bitcoin, cot.ethereum, cot.ripple);
        fclose(f);
    }
}

// Salva as informações do investidor no arquivo "investidor.txt"[cite: 4]
void salvarInvestidor(Investidor inv) {
    FILE *f = fopen("investidor.txt", "w");
    if (f != NULL) {
        fprintf(f, "%s\n%s\n%s\n%.2f\n%.6f\n%.6f\n%.6f\n",
                inv.nome, inv.cpf, inv.senha, inv.reais, inv.bitcoin, inv.ethereum, inv.ripple);
        fclose(f);
    }
}

// Efetua login verificando CPF e senha gravados em arquivo[cite: 7]
int efetuarLogin(Investidor *inv) {
    char cpfEntrada[15], senhaEntrada[10];

    FILE *f = fopen("investidor.txt", "r");
    if (f == NULL) {
        // Cria um usuario padrao de teste se o arquivo nao existir
        strcpy(inv->nome, "Pedro Braga");
        strcpy(inv->cpf, "00000000000");
        strcpy(inv->senha, "123456");
        inv->reais = 8000.00;
        inv->bitcoin = 0.017;
        inv->ethereum = 1.1;
        inv->ripple = 210.15;
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

// Registra uma operação no extrato texto com data/hora[cite: 9]
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

// Requisito 1: Consultar Saldo[cite: 8]
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

// Requisito 2: Consultar Extrato[cite: 9]
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

// Requisito 3: Depositar Reais[cite: 10]
void depositar(Investidor *inv) {
    double valor;
    printf("\nDigite o valor a depositar (R$): ");
    scanf("%lf", &valor);

    if (valor <= 0) {
        printf("Valor de deposito invalido!\n");
        return;
    }

    inv->reais += valor;
    salvarInvestidor(*inv);

    char desc[150];
    sprintf(desc, "+ %.2f REAL | REAL: %.2f BTC: %.4f ETH: %.4f XRP: %.4f", 
            valor, inv->reais, inv->bitcoin, inv->ethereum, inv->ripple);
    registrarExtrato(inv->cpf, desc);

    printf("\nDeposito de R$ %.2f realizado com sucesso!\n", valor);
    printf("Novo saldo em Reais: R$ %.2f\n", inv->reais);
}

// Requisito 4: Sacar Reais[cite: 11]
void sacar(Investidor *inv) {
    char senha[10];
    printf("\nDigite sua senha para confirmar: ");
    scanf("%s", senha);

    if (strcmp(senha, inv->senha) != 0) {
        printf("Senha incorreta!\n");
        return;
    }

    double valor;
    printf("Digite o valor do saque (R$): ");
    scanf("%lf", &valor);

    if (valor <= 0) {
        printf("Valor de saque invalido!\n");
        return;
    }

    if (valor > inv->reais) {
        printf("Saldo insuficiente para realizar o saque!\n");
        return;
    }

    inv->reais -= valor;
    salvarInvestidor(*inv);

    char desc[150];
    sprintf(desc, "- %.2f REAL | REAL: %.2f BTC: %.4f ETH: %.4f XRP: %.4f", 
            valor, inv->reais, inv->bitcoin, inv->ethereum, inv->ripple);
    registrarExtrato(inv->cpf, desc);

    printf("\nSaque de R$ %.2f realizado com sucesso!\n", valor);
    printf("Novo saldo em Reais: R$ %.2f\n", inv->reais);
}

// Requisito 5: Comprar Criptomoedas[cite: 5, 12]
void comprarCripto(Investidor *inv, Cotacoes cot) {
    char senha[10];
    printf("\nDigite sua senha para confirmar: ");
    scanf("%s", senha);

    if (strcmp(senha, inv->senha) != 0) {
        printf("Senha incorreta!\n");
        return;
    }

    printf("\n--- COTACOES ATUAIS ---\n");
    printf("1. Bitcoin  - R$ %.2f (Taxa: 2%%)\n", cot.bitcoin);
    printf("2. Ethereum - R$ %.2f (Taxa: 1%%)\n", cot.ethereum);
    printf("3. Ripple   - R$ %.2f (Taxa: 1%%)\n", cot.ripple);
    
    int opcao;
    printf("Escolha a moeda que deseja comprar (1-3): ");
    scanf("%d", &opcao);

    if (opcao < 1 || opcao > 3) {
        printf("Opcao invalida!\n");
        return;
    }

    double valorReais;
    printf("Digite o valor em REAIS que deseja investir: ");
    scanf("%lf", &valorReais);

    if (valorReais <= 0 || valorReais > inv->reais) {
        printf("Valor invalido ou saldo insuficiente em Reais!\n");
        return;
    }

    double taxaPct = 0.0, cotacao = 0.0;
    char moedanome[10];

    if (opcao == 1) {
        taxaPct = 0.02; // 2%
        cotacao = cot.bitcoin;
        strcpy(moedanome, "BTC");
    } else if (opcao == 2) {
        taxaPct = 0.01; // 1%
        cotacao = cot.ethereum;
        strcpy(moedanome, "ETH");
    } else if (opcao == 3) {
        taxaPct = 0.01; // 1%
        cotacao = cot.ripple;
        strcpy(moedanome, "XRP");
    }

    double taxaPaga = valorReais * taxaPct;
    double valorLiquido = valorReais - taxaPaga;
    double qtdCriptoComprada = valorLiquido / cotacao;

    inv->reais -= valorReais;
    if (opcao == 1) inv->bitcoin += qtdCriptoComprada;
    else if (opcao == 2) inv->ethereum += qtdCriptoComprada;
    else if (opcao == 3) inv->ripple += qtdCriptoComprada;

    salvarInvestidor(*inv);

    char desc[150];
    sprintf(desc, "+ %.6f %s CT: %.2f TX: %.2f REAL: %.2f BTC: %.4f ETH: %.4f XRP: %.4f",
            qtdCriptoComprada, moedanome, cotacao, taxaPaga, inv->reais, inv->bitcoin, inv->ethereum, inv->ripple);
    registrarExtrato(inv->cpf, desc);

    printf("\nCompra realizada com sucesso!\n");
    printf("Voce comprou %.6f %s (Taxa paga: R$ %.2f)\n", qtdCriptoComprada, moedanome, taxaPaga);
}

// Requisito 6: Vender Criptomoedas[cite: 5, 13]
void venderCripto(Investidor *inv, Cotacoes cot) {
    char senha[10];
    printf("\nDigite sua senha para confirmar: ");
    scanf("%s", senha);

    if (strcmp(senha, inv->senha) != 0) {
        printf("Senha incorreta!\n");
        return;
    }

    printf("\n--- COTACOES E TAXAS DE VENDA ---\n");
    printf("1. Bitcoin  - R$ %.2f (Taxa: 3%%) | Seu saldo: %.6f\n", cot.bitcoin, inv->bitcoin);
    printf("2. Ethereum - R$ %.2f (Taxa: 2%%) | Seu saldo: %.6f\n", cot.ethereum, inv->ethereum);
    printf("3. Ripple   - R$ %.2f (Taxa: 1%%) | Seu saldo: %.6f\n", cot.ripple, inv->ripple);

    int opcao;
    printf("Escolha a moeda que deseja vender (1-3): ");
    scanf("%d", &opcao);

    if (opcao < 1 || opcao > 3) {
        printf("Opcao invalida!\n");
        return;
    }

    double qtdVender;
    printf("Digite a quantidade de criptomoeda que deseja vender: ");
    scanf("%lf", &qtdVender);

    if (qtdVender <= 0) {
        printf("Quantidade invalida!\n");
        return;
    }

    double taxaPct = 0.0, cotacao = 0.0, *saldoCripto = NULL;
    char moedanome[10];

    if (opcao == 1) {
        taxaPct = 0.03; // 3%[cite: 5]
        cotacao = cot.bitcoin;
        saldoCripto = &inv->bitcoin;
        strcpy(moedanome, "BTC");
    } else if (opcao == 2) {
        taxaPct = 0.02; // 2%[cite: 5]
        cotacao = cot.ethereum;
        saldoCripto = &inv->ethereum;
        strcpy(moedanome, "ETH");
    } else if (opcao == 3) {
        taxaPct = 0.01; // 1%[cite: 5]
        cotacao = cot.ripple;
        saldoCripto = &inv->ripple;
        strcpy(moedanome, "XRP");
    }

    if (qtdVender > *saldoCripto) {
        printf("Saldo de criptomoeda insuficiente!\n");
        return;
    }

    double valorBruto = qtdVender * cotacao;
    double taxaPaga = valorBruto * taxaPct;
    double valorLiquido = valorBruto - taxaPaga;

    *saldoCripto -= qtdVender;
    inv->reais += valorLiquido;

    salvarInvestidor(*inv);

    char desc[150];
    sprintf(desc, "- %.6f %s CT: %.2f TX: %.2f REAL: %.2f BTC: %.4f ETH: %.4f XRP: %.4f",
            qtdVender, moedanome, cotacao, taxaPaga, inv->reais, inv->bitcoin, inv->ethereum, inv->ripple);
    registrarExtrato(inv->cpf, desc);

    printf("\nVenda realizada com sucesso!\n");
    printf("Voce vendeu %.6f %s por R$ %.2f (Taxa: R$ %.2f)\n", qtdVender, moedanome, valorLiquido, taxaPaga);
}

// Requisito 7: Atualizar Cotação
void atualizarCotacao(Cotacoes *cot) {
    srand(time(NULL));

    // Variação de -5% a +5%[cite: 14]
    double varBTC = ((rand() % 11) - 5) / 100.0;
    double varETH = ((rand() % 11) - 5) / 100.0;
    double varXRP = ((rand() % 11) - 5) / 100.0;

    cot->bitcoin += cot->bitcoin * varBTC;
    cot->ethereum += cot->ethereum * varETH;
    cot->ripple += cot->ripple * varXRP;

    salvarCotacoes(*cot);

    printf("\nCotacoes atualizadas com sucesso!\n");
    printf("Nova cotacao Bitcoin  : R$ %.2f (Variacao: %.1f%%)\n", cot->bitcoin, varBTC * 100);
    printf("Nova cotacao Ethereum : R$ %.2f (Variacao: %.1f%%)\n", cot->ethereum, varETH * 100);
    printf("Nova cotacao Ripple   : R$ %.2f (Variacao: %.1f%%)\n", cot->ripple, varXRP * 100);
}