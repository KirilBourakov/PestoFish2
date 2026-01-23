cutechess-cli ^
  -engine cmd=.\baseline/pestofish2.exe name=baseline ^
  -engine cmd=.\new/pestofish2.exe name=new ^
  -each proto=uci tc=10+0.1 ^
  -sprt elo0=0 elo1=5 alpha=0.05 beta=0.05 ^
  -games 1000 ^
  -repeat 2 ^
  -openings file=4plyopen.pgn plies=4 policy=round ^
  -concurrency 8

pause