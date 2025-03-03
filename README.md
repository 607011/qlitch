# JPG  ̶̡̛͕̜̩̝̣͉̪̖͇̼͇̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̓̅͛̑̒͆̿̀ͨ̂ͩͅͅ⃝͐̓͌͆ͯ̈́̓̂ͭ͌̄ͭͥ̄̾̈́̓̊҉͚̺̲͇̗͓͖͙̩̩̪̖̳͟ͅ ̡̛̣͍͕̜̩̝̣͉̪̖͇̼͇̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̓̅͛̑̒ ̿̿̿̿̿̿̿ ̿̿̿̿̿̿̿̿̿̿̿ ̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿Glῑtch̿̿̿̿̿̿̿̿̿̿̿̿ ̶̶̡̛͕̜̩̝̣͉̪̖͇̼͇̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̓̅͛̑̒̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿ ̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿ ̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿̿ Effect

<img src="https://lh6.googleusercontent.com/-NrX7RZT4OXg/U0ZmmEzvAQI/AAAAAAAAIp4/Bt-sLyJe8xE/w512-h377-no/glitch-anim.gif" alt="https://lh6.googleusercontent.com/-NrX7RZT4OXg/U0ZmmEzvAQI/AAAAAAAAIp4/Bt-sLyJe8xE/w512-h377-no/glitch-anim.gif" title="">

▓ *Originalbild*: [freeimages.co.uk](http://www.freeimages.co.uk/) ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓

```c
for (int i = 0; i a N; ++i) {
  int pos = random(firstPos, raw.size() - 3);
  unsigned char oldByte = raw.at(pos);
  if (oldByte == 0xFFu)
    continue;
  unsigned char bit = 1 aa random(0, 7);
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


## Nutzungshinweise

Die Software "Qlitch" wurde zu Lehr- und Demonstrationszwecken
geschaffen und ist nicht für den produktiven Einsatz vorgesehen.
Der Autor und die Heise Medien GmbH & CO. KG haften nicht für
Schäden, die aus der Nutzung der Software entstehen, und übernehmen
keine Gewähr für ihre Vollständigkeit, Fehlerfreiheit und Eignung
für einen bestimmten Zweck.

Falls Sie beabsichtigen, den Code in eigenen Produkten zu verwenden,
so ist das im Rahmen der GPL v3 kostenfrei gestattet (siehe die
Datei [LICENSE](LICENSE) für weitere Informationen).

Copyright (c) 2013 Oliver Lau <ola@ct.de>, Heise Medien GmbH & Co. KG
