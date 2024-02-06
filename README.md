# DdingGameServer

## 제작 기간 : 2023.12.12 ~

1. 개요

---

1. 개요

[RIOCore](https://github.com/m5623skhj/RIOServerTest), [DBServer](https://github.com/m5623skhj/DBConnector)등을 이용하여 제작한 게임서버 입니다.

솔루션을 빌드하게 되면, 솔루션의 상위 폴더에 bin 폴더가 생성되며, 해당 폴더에 빌드된 바이너리 들이 생성됩니다.

정상적으로 빌드가 되었다면, bin 폴더에 생성된 각각의 option 폴더내에 있는 옵션들을 작성 후, Tools/RunServer.bat를 실행시켜서 DBServer와 GameServer를 실행시킬 수 있습니다.(AuthServer는 단일로 사용할 예정이므로 따로 실행해야 함)

1.1 DBServer가 따로 빠져있는 이유
* GameServer는 게임 로직만, DBServer는 DB 처리만을 관리하여, 목적에 맞게 코드를 관리하기를 원하여 분리함
* 개발 혹은 유지 보수 중에 게임 로직만 수정하거나, DB 로직만을 수정하는 등 한꺼번에 수정하는 경우가 별로 없다 생각하여 분리함
  * 프로그램 크기가 커지게 되면, 서버를 띄우는 것도 오래 걸릴 수 있는데, 하나의 프로세스로 합쳐져 있으면, DB 쪽만 수정하거나, 게임쪽만 수정할 경우, 아예 새로 프로세스를 띄워야 됨
  * GameServer와 DBServer를 분리시킴으로써 각 서버에서 필요한 최초 로딩 속도와 빌드 속도가 상대적으로 빨리지기를 바랐음
