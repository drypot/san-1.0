# 한글 에디터 산 1.x

산은 DOS 용 한글 내장 에디터입니다.\
이곳은 산의 소스 코드 리포입니다.

브라우저 도스 에뮬레이터에서 실행해 보시려면.
<https://drypot.github.io/san-1.0-js-dos>

브라우저 도스 에뮬레이터 번들 리포.
<https://github.com/drypot/san-1.0-js-dos>

## 역사

1987년, 중학교 1학년.  
Apple CP/M 에서 Turbo Pascal 을 쓰기 시작했습니다.  
CP/M 에서 이런 저런 에디터를 만들면서 놀았습니다.  
만든 것을 디스켓에 넣어 세운상가 아저씨들에게 보여드렸습니다.  


1990년, 고등학교 1학년.  
IBM AT, DOS 에서 Turbo C 를 공부하기 시작했습니다.  
같은 해 C 로 Trance 라는 도스 램상주 에디터를 만들었습니다.  
1회 월간 마이크로소프트 공모전에 출품했습니다. 2등.

1991년, 고등학교 2학년.  
Trance 에서 램상주 기능을 제거했습니다.  
직접 만든 한글 입출력 라이브러리를 붙였습니다.  
이름을 한글 에디터 산(SAN)이라 해서 배포하기 시작했습니다.

1993년, 대학교 1학년.  
구조를 완전히 새로 개선한 산 2.x를 만들기 시작했습니다.  
산 1.x 코드는 공개했습니다.  
지금 보고 계신 이 코드입니다.

1994년, 대학교 2학년.  
산 2.x 버젼을 배포하기 시작했습니다.  
무제한 편집 용량, 무제한 Undo/Redo, 내장 매크로 언어,  
DOS, OS/2 동시 지원등.  

하지만 아버지 돌아가시고 대학등록금을 직접 벌어야 했습니다.  
다행히 인터넷 붐이 터졌습니다.  
웹 코딩을 시작했습니다.   
산은 더이상 개발하지 못했습니다.

## 코드 설명

저도 기억이 정확하지 않습니다.  
써놓은 글을 보니 1993년 9월 처음 코드를 공개한 것 같습니다.  
DOC/SRC.TXT 참고.

그러다 1년 후 1994년 9월,  
바이너리와 폰트, 소스 코드를 정리해서 한번 더 배포한 것 같습니다.  
DOC/README.TXT 참고.

그러니 이 리포지터리의 코드는 1994년 배포한 산 1.2.1 일 것입니다;  
코드 주석에는 Trance 0.5 라는 단어가 많이 남아있는데 무시해 주십시오;

저도 제 코드를 망실해서 가지고 있지 못했습니다.  
인터넷에 돌아다니던 코드와 글들을 다시 받아서 폴더 구조를 정리한 후  
GitHub 에 올리는 중입니다.

당시 쓴 DOC 문서들은 euc-kr 완성형 한글로 인코딩되어 있었습니다.  
편의를 위해 UTF8 로 변환했습니다.

지금 보니 소스 코드 안에도 한글이 많이 들어있습니다.  
조합형 한글인 것 같습니다.  
UTF8 유닉스 환경에서 조합형 한글을 읽으려면 쉘에서 iconv 를 사용하시면 됩니다.

    $ iconv -c -f JOHAB -t utf-8 < something.txt

## 실행

binary, font-kor, font-eng 디렉토리를 한 디렉토리에 모두 복사해야 실행이 됩니다.

당시 산을 개발하고 배포할 때는 머리에 폴더 개념이 없었습니다.  
실행에 필요한 모든 파일과 EXE 바이너리가 한 디렉토리에 있었습니다.  
지저분해서 리포지터리에는 폴더를 만들고 관련 파일들을 구분해 넣었습니다.  
그러니 현재 리포지터리 구조로는 실행되지 않습니다.

## 주의

당시 철부지 고등학생이 쓴 문서들이 곳곳에 남아있습니다.  
유치하니 주의해 주십시오;

## License

[MIT](LICENSE)
