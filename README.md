# JPG  ̶̡̛͕̜̩̝̣͉̪̖͇̼͇̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̓̅͛̑̒͆̿̀ͨ̂ͩͅͅ⃝͐̓͌͆ͯ̈́̓̂ͭ͌̄ͭͥ̄̾̈́̓̊҉͚̺̲͇̗͓͖͙̩̩̪̖̳͟ͅ ̡̛̣͍͕̜̩̝̣͉̪̖͇̼͇̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̓̅͛̑̒ ̿̿̿̿̿̿̿ ̿̿̿̿̿̿̿̿̿̿̿ ̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿Glῑtch̿̿̿̿̿̿̿̿̿̿̿̿ ̶̶̡̛͕̜̩̝̣͉̪̖͇̼͇̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̓̅͛̑̒̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿ ̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿ ̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿ Effect

<img src="https://lh6.googleusercontent.com/-NrX7RZT4OXg/U0ZmmEzvAQI/AAAAAAAAIp4/Bt-sLyJe8xE/w512-h377-no/glitch-anim.gif" alt="https://lh6.googleusercontent.com/-NrX7RZT4OXg/U0ZmmEzvAQI/AAAAAAAAIp4/Bt-sLyJe8xE/w512-h377-no/glitch-anim.gif" title="">
▓ _Originalbild_: [freeimages.co.uk](http://www.freeimages.co.uk/) ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓

```c
for (int i = 0; i &lt; N; ++i) {
  int pos = random(firstPos, raw.size() - 3);
  unsigned char oldByte = raw.at(pos);
  if (oldByte == 0xFFu)
    continue;
  unsigned char bit = 1 &lt;&lt; random(0, 7);
  switch (d->algorithm) {
  case ALGORITHM_XOR:
    newByte = oldByte ^ bit;
    break;
  case ALGORITHM_ONE:
    newByte = oldByte | bit;
    break;
  case ALGORITHM_ZERO:
    newByte = oldByte &amp; ~bit;
    break;
  }
  raw[pos] = (newByte == 0xFFu)? oldByte : newByte;
}
```

`/**` Dinge kaputt machen muss nicht deren Zerstörung bedeuten, sondern kann auch Ausdruck von Kreativitᾄt sein. Sehr beliebt ist der Gli†ch-Effekt in JPᴳ̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇̇-Bildern, für den es sogar teure Photoshop-Plug-ins gibt. Das Geld können Sie sich sparen. ▀▀▄  
Für weitere Infos siehe c’t 2/2014, S. 172 `*/`
