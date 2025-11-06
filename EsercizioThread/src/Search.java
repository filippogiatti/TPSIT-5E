public class Search extends Thread{
    private int[] v; // vettore con i numeri
    private int a; //indice minimo
    private int b; // indice massimo
    private int e; //elemento da ricercare
    private int n; //conteggio occorenze

    public Search(int[] v, int a, int b, int e, int n) {
        this.v = v;
        this.a = a;
        this.b = b;
        this.e = e;
        n=0;
    }

    public int conteggio(){
        return n;
    }

    @Override
    public void run(){
        for(int i=a; i<b; i++){
            if(v[i]==e){
                n++;
            }
        }
    }
}
