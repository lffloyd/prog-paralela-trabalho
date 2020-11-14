# prog-paralela-trabalho
[![Actions Status](https://github.com/lffloyd/prog-paralela-trabalho/workflows/CMake/badge.svg)](https://github.com/lffloyd/prog-paralela-trabalho/actions)

Trabalho de implementação da disciplina de Laboratório de Programação Paralela do IC/UFF, período 2020.1.

## Ambiente
* **[cmake >= 3.18](https://cmake.org/install/)** - framework de build do projeto. Para usar a versão mais recente, [faça o build a partir do source](https://cmake.org/download/).

## Instalação
Execute ```./install.sh``` para obter as dependências do projeto (Google Test).

Para executar os scripts Python, será necessário executar ```./python_install.sh``` para obter as dependências. É necessário possuir Python 3 e Pip pré-instalados para executar tal ação.

## Build
Execute ```./build.sh``` para construir o código. Os executáveis serão gerados na pasta ```build``` na raiz do projeto.
Testes unitários serão executados automaticamente ao realizar o build.

## Resultados de execução
Cada implementação (Sequencial, OpenMP e MPI) é executada a partir de um comando distinto, mas com o mesmo executável.

Ao executada, todas as versões irão gerar seus resultados de tempos de execução em formato CSV no diretório ```results``` na raiz deste projeto.

Após executar a(s) implementação(ões), a geração de gráficos a partir das tabelas geradas pode ser realizada com o comando:

```python3 src/graphs/graphs.py arquivo1.csv arquivo2.csv arquivo3.csv ...```, onde os argumentos da chamada são os nomes dos arquivos CSV gerados na etapa anterior. Os gráficos serão gerados no formato PNG, também no diretório ```results/graphs```.

### Executando a versão sequencial
Para executar a versão sequencial do Prim, execute:

```./build/src/ProgParalelaTrabalho_run SEQ <lista de tamanhos de matriz quadrada>```, onde a lista de argumentos são inteiros representando os tamanhos n de matrizes a serem geradas para a coleta dos tempos.

Um exemplo de possível execução para o comando é ilustrado a seguir:
```./build/src/ProgParalelaTrabalho_run SEQ 100 1000 10000 100000```

### Executando a versão em OpenMP
Para executar a versãoem OpenMP do Prim, execute:

```./build/src/ProgParalelaTrabalho_run OMP <lista de tamanhos de matriz quadrada> <num threads>```, onde a lista de argumentos são inteiros representando os tamanhos n de matrizes a serem geradas para a coleta dos tempos, e o último argumento será um inteiro representando o número de threads a serem usadas para execução.

Um exemplo de possível execução para o comando é ilustrado a seguir:
```./build/src/ProgParalelaTrabalho_run OMP 100 1000 10000 100000 5```, onde a execução será realizada com 5 threads.

### Executando a versão em MPI
Para executar a versão em MPI do Prim, execute:

```mpiexec -np <num processos> ./build/src/ProgParalelaTrabalho_run MPI <lista de tamanhos de matriz quadrada>```, onde o número de processos é passado como o parâmetro ```-np``` e por fim são passados os inteiros representando os tamanhos n de matrizes a serem geradas para a coleta dos tempos.

Um exemplo de possível execução para o comando é ilustrado a seguir:
```mpiexec -np 4 ./build/src/ProgParalelaTrabalho_run MPI 100 1000 10000 100000```, onde a execução será realizada com 4 processos.

### Links
* [Estruturando um projeto com CMake e testes unitários](https://raymii.org/s/tutorials/Cpp_project_setup_with_cmake_and_unit_tests.html)
* [Google Test Primer](https://github.com/google/googletest/blob/master/googletest/docs/primer.md)
