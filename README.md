# Sistema de Gerenciamento de Emergências

> Este repositório contém um sistema que realiza a priorização de pacientes conforme o nível de urgência, além de realizar a busca do histórico do paciente conforme necessário. 

Uma maneira de implementar um sistema para solucionar o problema de gerenciamento de
emergências do hospital se baseia em duas principais ideias:

- **Chamadas de emergência:** uma lista de prioridades é usada para gerenciar as chamadas de emergência, onde cada chamada é classificada de acordo com o nível de gravidade. A gravidade é atribuída como um valor numérico: quanto maior o número, maior a gravidade. Assim, o sistema precisa sempre atender primeiro as chamadas mais críticas. Ou seja, as chamadas de emergência são armazenadas na lista de prioridades e a operação principal é a *inserção* de novas chamadas e a *extração* do paciente mais urgente.

- **Registros médicos:** uma tabela de dispersão é usada para armazenar os registros médicos dos pacientes, permitindo uma busca rápida pelo seu histórico, onde a chave é o número de identificação do paciente, mais outras informações como nome e enfermidade.

## Fluxo do sistema

1. Um paciente faz uma chamada de emergência.
2. A gravidade da emergência é avaliada e inserida na lista de prioridades.
3. O sistema prioriza o atendimento de acordo com a gravidade, sempre atendendo o caso mais urgente.
