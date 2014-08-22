int spellCheck(String Buf){

  int n = Buf.length();
  int i = 0;
  char c;
  int result = 0;

  while(i<n){
    c = Buf.charAt(i);
    if(('0'>c || c>'9')&&c!='.'){return -1;}
    i++;
  }
  return 0;
}

void setup(){
  Serial.begin(9600);
}
void loop(){
  String A = "";
  while(Serial.available()>0){
    char c = Serial.read();delay(5);
    A += c;
  }
  if(A!="")
  {
    int i = spellCheck(A);
    Serial.println("spell check: ");
    Serial.print(i);
  }
}
