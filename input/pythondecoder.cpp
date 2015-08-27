#include "pythondecoder.h"


pythondecoder::pythondecoder() {}



void pythondecoder::init() {

    ok = false;
    buffer = "";
    conf = "BBBBBBBBiiHHHhhhhhhhhhh";

    rs = init_rs_char(8, 0x11d, 0, 1, 20);
    if(rs != NULL) {
        ok = true;
        emit message("Init decoder ok");
    } else {
        emit message("Init decoder failed");
    }

    startByte = 255;

    frame_length = calcframelength();

    qDebug() << "Frame length: " << frame_length;
}

pythondecoder::~pythondecoder() {
}

#define REEDSOLO

bool pythondecoder::decode(QByteArray frame) {
    if(!ok)
        return false;



qDebug() << QString(frame.toHex());

#ifdef REEDSOLO

    int result = decode_rs_char(rs, (unsigned char*)frame.data(), NULL, 0);
    if (result == -1) {
        emit trame_erreur(1);
        emit message("Erreur de correction");
    } else {
        emit trame_corrigee(1);
        emit message("OK!");
    }
#endif

    //emit newFrame(unpacker.unpack(conf,frame.mid(1,content_size)));

    return true;
}

int pythondecoder::getrslength() {
    return 20;
}

int pythondecoder::calcframelength() {
    content_size = unpacker.calcsize(conf);
    return 255;//2+content_size+getrslength();
}

void pythondecoder::appendData(QByteArray received) {
    if(!ok)
        return;

    buffer.append(received);
    qDebug() << "Received:" << QString(received.toHex())<< " || buffer:"<<QString(buffer.toHex());


    while(buffer.indexOf(startByte) != -1 && (buffer.length() - buffer.indexOf(startByte)) >= frame_length) {
        int firstStart = buffer.indexOf(startByte);
        QByteArray frame_to_decode = buffer.mid(firstStart, frame_length);
        bool result = decode(frame_to_decode);
        qDebug() << "result: "<< (result? " true":"false");
        if(result)
            buffer = buffer.mid(firstStart + frame_length);
        else if (buffer.indexOf(startByte,firstStart+1) != -1)
            buffer = buffer.mid(buffer.indexOf(startByte,firstStart+1));
        else
            buffer = "";

        qDebug() << "New buffer: " << QString(buffer.toHex());
    }

}


#define NULL ((void *)0)

#define DEBUG 2

void FREE_RS(void *p){
  struct rs *rs = (struct rs *)p;

  free(rs->alpha_to);
  free(rs->index_of);
  free(rs->genpoly);
  free(rs);
}

/* Initialize a Reed-Solomon codec
 * symsize = symbol size, bits (1-8)
 * gfpoly = Field generator polynomial coefficients
 * fcr = first root of RS code generator polynomial, index form
 * prim = primitive element to generate polynomial roots
 * nroots = RS code generator polynomial degree (number of roots)
 */
void *INIT_RS(unsigned int symsize,unsigned int gfpoly,unsigned fcr,unsigned prim,
        unsigned int nroots){
  struct rs *rs;
  int i, j, sr,root,iprim;

  if(symsize > 8*sizeof(DTYPE))
    return NULL; /* Need version with ints rather than chars */

  if(fcr >= (1<<symsize))
    return NULL;
  if(prim == 0 || prim >= (1<<symsize))
    return NULL;
  if(nroots >= (1<<symsize))
    return NULL; /* Can't have more roots than symbol values! */

  rs = (struct rs *)calloc(1,sizeof(struct rs));
  rs->mm = symsize;
  rs->nn = (1<<symsize)-1;

  rs->alpha_to = (DTYPE *)malloc(sizeof(DTYPE)*(rs->nn+1));
  if(rs->alpha_to == NULL){
    free(rs);
    return NULL;
  }
  rs->index_of = (DTYPE *)malloc(sizeof(DTYPE)*(rs->nn+1));
  if(rs->index_of == NULL){
    free(rs->alpha_to);
    free(rs);
    return NULL;
  }

  /* Generate Galois field lookup tables */
  rs->index_of[0] = A0; /* log(zero) = -inf */
  rs->alpha_to[A0] = 0; /* alpha**-inf = 0 */
  sr = 1;
  for(i=0;i<rs->nn;i++){
    rs->index_of[sr] = i;
    rs->alpha_to[i] = sr;
    sr <<= 1;
    if(sr & (1<<symsize))
      sr ^= gfpoly;
    sr &= rs->nn;
  }
  if(sr != 1){
    /* field generator polynomial is not primitive! */
    free(rs->alpha_to);
    free(rs->index_of);
    free(rs);
    return NULL;
  }

  /* Form RS code generator polynomial from its roots */
  rs->genpoly = (DTYPE *)malloc(sizeof(DTYPE)*(nroots+1));
  if(rs->genpoly == NULL){
    free(rs->alpha_to);
    free(rs->index_of);
    free(rs);
    return NULL;
  }
  rs->fcr = fcr;
  rs->prim = prim;
  rs->nroots = nroots;

  /* Find prim-th root of 1, used in decoding */
  for(iprim=1;(iprim % prim) != 0;iprim += rs->nn)
    ;
  rs->iprim = iprim / prim;

  rs->genpoly[0] = 1;
  for (i = 0,root=fcr*prim; i < nroots; i++,root += prim) {
    rs->genpoly[i+1] = 1;

    /* Multiply rs->genpoly[] by  @**(root + x) */
    for (j = i; j > 0; j--){
      if (rs->genpoly[j] != 0)
    rs->genpoly[j] = rs->genpoly[j-1] ^ rs->alpha_to[modnn(rs,rs->index_of[rs->genpoly[j]] + root)];
      else
    rs->genpoly[j] = rs->genpoly[j-1];
    }
    /* rs->genpoly[0] can never be zero */
    rs->genpoly[0] = rs->alpha_to[modnn(rs,rs->index_of[rs->genpoly[0]] + root)];
  }
  /* convert rs->genpoly[] to index form for quicker encoding */
  for (i = 0; i <= nroots; i++)
    rs->genpoly[i] = rs->index_of[rs->genpoly[i]];

  return rs;
}

#define NULL ((void *)0)
#define	min(a,b)	((a) < (b) ? (a) : (b))

int DECODE_RS(
#ifndef FIXED
void *p,
#endif
DTYPE *data, int *eras_pos, int no_eras){

#ifndef FIXED
  struct rs *rs = (struct rs *)p;
#endif
  int deg_lambda, el, deg_omega;
  int i, j, r,k;
  DTYPE u,q,tmp,num1,num2,den,discr_r;
  DTYPE lambda[NROOTS+1], s[NROOTS];	/* Err+Eras Locator poly
                     * and syndrome poly */
  DTYPE b[NROOTS+1], t[NROOTS+1], omega[NROOTS+1];
  DTYPE root[NROOTS], reg[NROOTS+1], loc[NROOTS];
  int syn_error, count;

  /* form the syndromes; i.e., evaluate data(x) at roots of g(x) */
  for(i=0;i<NROOTS;i++)
    s[i] = data[0];

  for(j=1;j<NN;j++){
    for(i=0;i<NROOTS;i++){
      if(s[i] == 0){
    s[i] = data[j];
      } else {
    s[i] = data[j] ^ ALPHA_TO[MODNN(INDEX_OF[s[i]] + (FCR+i)*PRIM)];
      }
    }
  }

  /* Convert syndromes to index form, checking for nonzero condition */
  syn_error = 0;
  for(i=0;i<NROOTS;i++){
    syn_error |= s[i];
    s[i] = INDEX_OF[s[i]];
  }

  if (!syn_error) {
    /* if syndrome is zero, data[] is a codeword and there are no
     * errors to correct. So return data[] unmodified
     */
    count = 0;
    goto finish;
  }
  memset(&lambda[1],0,NROOTS*sizeof(lambda[0]));
  lambda[0] = 1;

  if (no_eras > 0) {
    /* Init lambda to be the erasure locator polynomial */
    lambda[1] = ALPHA_TO[MODNN(PRIM*(NN-1-eras_pos[0]))];
    for (i = 1; i < no_eras; i++) {
      u = MODNN(PRIM*(NN-1-eras_pos[i]));
      for (j = i+1; j > 0; j--) {
    tmp = INDEX_OF[lambda[j - 1]];
    if(tmp != A0)
      lambda[j] ^= ALPHA_TO[MODNN(u + tmp)];
      }
    }

#if DEBUG >= 1
    /* Test code that verifies the erasure locator polynomial just constructed
       Needed only for decoder debugging. */

    /* find roots of the erasure location polynomial */
    for(i=1;i<=no_eras;i++)
      reg[i] = INDEX_OF[lambda[i]];

    count = 0;
    for (i = 1,k=IPRIM-1; i <= NN; i++,k = MODNN(k+IPRIM)) {
      q = 1;
      for (j = 1; j <= no_eras; j++)
    if (reg[j] != A0) {
      reg[j] = MODNN(reg[j] + j);
      q ^= ALPHA_TO[reg[j]];
    }
      if (q != 0)
    continue;
      /* store root and error location number indices */
      root[count] = i;
      loc[count] = k;
      count++;
    }
    if (count != no_eras) {
      printf("count = %d no_eras = %d\n lambda(x) is WRONG\n",count,no_eras);
      count = -1;
      goto finish;
    }
#if DEBUG >= 2
    printf("\n Erasure positions as determined by roots of Eras Loc Poly:\n");
    for (i = 0; i < count; i++)
      printf("%d ", loc[i]);
    printf("\n");
#endif
#endif
  }
  for(i=0;i<NROOTS+1;i++)
    b[i] = INDEX_OF[lambda[i]];

  /*
   * Begin Berlekamp-Massey algorithm to determine error+erasure
   * locator polynomial
   */
  r = no_eras;
  el = no_eras;
  while (++r <= NROOTS) {	/* r is the step number */
    /* Compute discrepancy at the r-th step in poly-form */
    discr_r = 0;
    for (i = 0; i < r; i++){
      if ((lambda[i] != 0) && (s[r-i-1] != A0)) {
    discr_r ^= ALPHA_TO[MODNN(INDEX_OF[lambda[i]] + s[r-i-1])];
      }
    }
    discr_r = INDEX_OF[discr_r];	/* Index form */
    if (discr_r == A0) {
      /* 2 lines below: B(x) <-- x*B(x) */
      memmove(&b[1],b,NROOTS*sizeof(b[0]));
      b[0] = A0;
    } else {
      /* 7 lines below: T(x) <-- lambda(x) - discr_r*x*b(x) */
      t[0] = lambda[0];
      for (i = 0 ; i < NROOTS; i++) {
    if(b[i] != A0)
      t[i+1] = lambda[i+1] ^ ALPHA_TO[MODNN(discr_r + b[i])];
    else
      t[i+1] = lambda[i+1];
      }
      if (2 * el <= r + no_eras - 1) {
    el = r + no_eras - el;
    /*
     * 2 lines below: B(x) <-- inv(discr_r) *
     * lambda(x)
     */
    for (i = 0; i <= NROOTS; i++)
      b[i] = (lambda[i] == 0) ? A0 : MODNN(INDEX_OF[lambda[i]] - discr_r + NN);
      } else {
    /* 2 lines below: B(x) <-- x*B(x) */
    memmove(&b[1],b,NROOTS*sizeof(b[0]));
    b[0] = A0;
      }
      memcpy(lambda,t,(NROOTS+1)*sizeof(t[0]));
    }
  }

  /* Convert lambda to index form and compute deg(lambda(x)) */
  deg_lambda = 0;
  for(i=0;i<NROOTS+1;i++){
    lambda[i] = INDEX_OF[lambda[i]];
    if(lambda[i] != A0)
      deg_lambda = i;
  }
  /* Find roots of the error+erasure locator polynomial by Chien search */
  memcpy(&reg[1],&lambda[1],NROOTS*sizeof(reg[0]));
  count = 0;		/* Number of roots of lambda(x) */
  for (i = 1,k=IPRIM-1; i <= NN; i++,k = MODNN(k+IPRIM)) {
    q = 1; /* lambda[0] is always 0 */
    for (j = deg_lambda; j > 0; j--){
      if (reg[j] != A0) {
    reg[j] = MODNN(reg[j] + j);
    q ^= ALPHA_TO[reg[j]];
      }
    }
    if (q != 0)
      continue; /* Not a root */
    /* store root (index-form) and error location number */
#if DEBUG>=2
    printf("count %d root %d loc %d\n",count,i,k);
#endif
    root[count] = i;
    loc[count] = k;
    /* If we've already found max possible roots,
     * abort the search to save time
     */
    if(++count == deg_lambda)
      break;
  }
  if (deg_lambda != count) {
    /*
     * deg(lambda) unequal to number of roots => uncorrectable
     * error detected
     */
    count = -1;
    goto finish;
  }
  /*
   * Compute err+eras evaluator poly omega(x) = s(x)*lambda(x) (modulo
   * x**NROOTS). in index form. Also find deg(omega).
   */
  deg_omega = 0;
  for (i = 0; i < NROOTS;i++){
    tmp = 0;
    j = (deg_lambda < i) ? deg_lambda : i;
    for(;j >= 0; j--){
      if ((s[i - j] != A0) && (lambda[j] != A0))
    tmp ^= ALPHA_TO[MODNN(s[i - j] + lambda[j])];
    }
    if(tmp != 0)
      deg_omega = i;
    omega[i] = INDEX_OF[tmp];
  }
  omega[NROOTS] = A0;

  /*
   * Compute error values in poly-form. num1 = omega(inv(X(l))), num2 =
   * inv(X(l))**(FCR-1) and den = lambda_pr(inv(X(l))) all in poly-form
   */
  for (j = count-1; j >=0; j--) {
    num1 = 0;
    for (i = deg_omega; i >= 0; i--) {
      if (omega[i] != A0)
    num1  ^= ALPHA_TO[MODNN(omega[i] + i * root[j])];
    }
    num2 = ALPHA_TO[MODNN(root[j] * (FCR - 1) + NN)];
    den = 0;

    /* lambda[i+1] for i even is the formal derivative lambda_pr of lambda[i] */
    for (i = min(deg_lambda,NROOTS-1) & ~1; i >= 0; i -=2) {
      if(lambda[i+1] != A0)
    den ^= ALPHA_TO[MODNN(lambda[i+1] + i * root[j])];
    }
    if (den == 0) {
#if DEBUG >= 1
      printf("\n ERROR: denominator = 0\n");
#endif
      count = -1;
      goto finish;
    }
    /* Apply error to data */
    if (num1 != 0) {
      data[loc[j]] ^= ALPHA_TO[MODNN(INDEX_OF[num1] + INDEX_OF[num2] + NN - INDEX_OF[den])];
    }
  }
 finish:
  if(eras_pos != NULL){
    for(i=0;i<count;i++)
      eras_pos[i] = loc[i];
  }
  return count;
}

