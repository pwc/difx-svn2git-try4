      SUBROUTINE VECAD ( A, B, C )
      IMPLICIT None
C 
C 1.    VECAD 
C 
C 1.1   VECAD PROGRAM SPECIFICATION 
C 
C 1.1.1 VECAD IS THE VECTOR UTILITY ROUTINE WHICH ADDS TOGETHER 
C       TWO VECTORS PRODUCING A SUM VECTOR. 
C 
C 1.1.2 RESTRICTIONS - NONE 
C 
C 1.1.3 REFERENCES - NONE 
C 
C 1.2   VECAD PROGRAM INTERFACE 
C 
C 1.2.1 CALLING SEQUENCE -
C 
C           INPUT VARIABLES:
C             1.  A(3,3)  -  THE FIRST VECTOR.
C             2.  B(3,3)  -  THE SECOND VECTOR. 
C 
C           OUTPUT VARIABLES: 
C             1.  C(3,3)  -  THE SUM OF VECTORS A AND B.
C 
C 1.2.2 COMMON BLOCKS USED -
C 
      INCLUDE 'ccon.i'             
C 
C            VARIABLES "FROM":
C              1.  KVECC  -  THE VECTOR UTILITY ROUTINE FLOW CONTROL FLAG.
C              2.  KVECD  -  THE VECTOR UTILITY ROUTINE DEBUG OUTPUT FLAG.
C 
C            VARIABLES "TO": NONE 
C 
C 1.2.3 PROGRAM SPECIFICATIONS -
C 
      Real*8 A(3), B(3), C(3) 
      Integer*4 I
C 
C 1.2.4 DATA BASE ACCESS - NONE 
C 
C 1.2.5 EXTERNAL INPUT/OUTPUT - POSSIBLE DEBUG OUTPUT
C 
C 1.2.6 SUBROUTINE INTERFACE -
C             CALLER SUBROUTINES: ETDG, ETDP, SITCR, STRCR
C             CALLED SUBROUTINES: NONE
C 
C 1.2.7 CONSTANTS USED - NONE 
C 
C 1.2.8 PROGRAM VARIABLES - NONE
C 
C 1.2.9 PROGRAMMER - DALE MARKHAM  01/19/77 
C                    PETER DENATALE 07/18/77
C                    SAVITA GOEL    06/04/97 (CDS FOR A900) 
C                    David Gordon 94.04.18 Converted to Implicit None.
C 
C 1.3   VECAD PROGRAM STRUCTURE 
C 
C   1.    PERFORM THE ADDITION. 
C 
      DO 100  I = 1,3 
           C(I) = A(I) + B(I) 
  100 CONTINUE
C 
C   2.    CHECK KVECD TO DETERMINE IF DEBUG OUTPUT IS NECESSARY.
C 
      IF ( KVECD .EQ. 0 )  GO TO 300
C 
      WRITE ( 6, 9100 )
 9100 FORMAT (1X, "DEBUG OUTPUT FOR SUBROUTINE VECAD." )
C 
      WRITE ( 6, 9200 )  A, B, C 
 9200 FORMAT (1X, "A   = ", 3 ( D30.16, 10X ), /, 1X, 
     1            'B   = ', 3 ( D30.16, 10X ), /, 1X, 
     2            'C   = ', 3 ( D30.16, 10X ) ) 
C 
C   3.    NORMAL PROGRAM CONCLUSION.
C 
  300 RETURN
      END 
C*****************************************************************************
      SUBROUTINE VECSB ( A, B, C )
      IMPLICIT None
C 
C 2.    VECSB 
C 
C 2.1   VECSB PROGRAM SPECIFICATION 
C 
C 2.1.1 VECSB IS THE VECTOR UTILITY ROUTINE WHICH SUBTRACTS 
C       TWO VECTORS PRODUCING A DIFFERENCE VECTOR.
C 
C 2.1.2 RESTRICTIONS - NONE 
C 
C 2.1.3 REFERENCES - NONE 
C 
C 2.2   VECSB PROGRAM INTERFACE 
C 
C 2.2.1 CALLING SEQUENCE -
C 
C           INPUT VARIABLES:
C             1.  A(3,3)  -  THE FIRST VECTOR.
C             2.  B(3,3)  -  THE SECOND VECTOR. 
C 
C           OUTPUT VARIABLES: 
C             1.  C(3,3)  -  THE DIFFERENCE OF VECTORS A AND B. 
C 
C 2.2.2 COMMON BLOCKS USED -
C 
      INCLUDE 'ccon.i'               
C 
C            VARIABLES "FROM":
C              1.  KVECC  -  THE VECTOR UTILITY ROUTINE FLOW CONTROL FLAG.
C              2.  KVECD  -  THE VECTOR UTILITY ROUTINE DEBUG OUTPUT FLAG.
C 
C            VARIABLES "TO": NONE 
C 
C 2.2.2 PROGRAM SPECIFICATIONS -
C 
      Real*8  A(3), B(3), C(3) 
      Integer*4 I
C 
C 2.2.4 DATA BASE ACCESS - NONE 
C 
C 2.2.5 EXTERNAL INPUT/OUTPUT -  POSSIBLE DEBUG OUTPUT
C 
C 2.2.6 SUBROUTINE INTERFACE -
C             CALLER SUBROUTINES: ETDC, ETDG, SITCR, SITG 
C             CALLED SUBROUTINES: NONE
C 
C 2.2.7 CONSTANTS USED - NONE 
C 
C 2.2.8 PROGRAM VARIABLES - NONE
C 
C 2.2.9 PROGRAMMER - DALE MARKHAM  01/19/77 
C                    PETER DENATALE 07/18/77
C                    David Gordon 94.04.18 Converted to Implicit None.
C 
C 2.3   VECSB PROGRAM STRUCTURE 
C 
C   1.    PERFORM THE SUBTRACTION.
C 
      DO 100  I = 1,3 
           C(I) = A(I) - B(I) 
  100 CONTINUE
C 
C   2.    CHECK KVECD TO DETERMINE IF DEBUG OUTPUT IS NECESSARY.
C 
      IF ( KVECD .EQ. 0 )  GO TO 300
C 
      WRITE ( 6, 9100 )
 9100 FORMAT (1X, "DEBUG OUTPUT FOR SUBROUTINE VECSB." )
C 
      WRITE ( 6, 9200 )  A, B, C 
 9200 FORMAT (1X, "A    = ", 3 ( D30.16, 10X ), /, 1X,
     1            'B    = ', 3 ( D30.16, 10X ), /, 1X,
     2            'C    = ', 3 ( D30.16, 10X ) )
C 
C   3.    NORMAL PROGRAM CONCLUSION.
C 
  300 RETURN
      END 
C******************************************************************************
      Real*8 FUNCTION DOTP ( A, B )
      IMPLICIT None
C 
C 3.    DOTP
C 
C 3.1   DOTP PROGRAM SPECIFICATION
C 
C 3.1.1 DOTP IS THE VECTOR UTILITY ROUTINE WHICH COMPUTES THE DOT 
C       PRODUCT BETWEEN TWO VECTORS.
C 
C 3.1.2 RESTRICTIONS - NONE 
C 
C 3.1.3 REFERENCES - NONE 
C 
C 3.2   DOTP PROGRAM INTERFACE
C 
C 3.2.1 CALLING SEQUENCE - NONE 
C 
C           INPUT VARIABLES:
C             1.  A(3)  -  THE FIRST VECTOR.
C             2.  B(3)  -  THE SECOND VECTOR. 
C
C           OUTPUT VARIABLES: 
C             1.  DOTP  -  THE DOT PRODUCT BETWEEN VECTORS A AND B. 
C 
C 3.2.2 COMMON BLOCKS USED -
C 
      INCLUDE 'ccon.i'             
C 
C            VARIABLES "FROM":
C              1.  KVECC  -  THE VECTOR UTILITY ROUTINE FLOW CONTROL FLAG.
C              2.  KVECD  -  THE VECTOR UTILITY ROUTINE DEBUG OUTPUT FLAG.
C 
C            VARIABLES "TO": NONE 
C 
C 3.2.3 PROGRAM SPECIFICATIONS -
C 
      Real*8 A(3), B(3) 
C 
C 3.2.4 DATA BASE ACCESS - NONE 
C 
C 3.2.5 EXTERNAL INPUT/OUTPUT - POSSIBLE DEBUG OUTPUT
C 
C 3.2.6 SUBROUTINE INTERFACE -
C             CALLER SUBROUTINES: AXOG, ETDC, ETDG, ETDP, PREP, RELP, 
C                                 SITP, SUNCR, UT1P, WOBP, etc., etc. 
C             CALLED SUBROUTINES: NONE
C 
C 3.2.7 CONSTANTS USED - NONE 
C 
C 3.2.8 PROGRAM VARIABLES - NONE
C 
C 3.2.9 PROGRAMMER - DALE MARKHAM  01/19/77 
C                    PETER DENATALE 07/18/77
C                    David Gordon 94.04.18 Converted to Implicit None.
C 
C 3.3   DOTP PROGRAM STRUCTURE
C 
C   1.    COMPUTE THE DOT PRODUCT.
C 
      DOTP = A(1) * B(1)
     1     + A(2) * B(2)
     2     + A(3) * B(3)
C 
C   2.    CHECK KVECD TO DETERMINE IF DEBUG OUTPUT IS NECESSARY.
C 
      IF ( KVECD .EQ. 0 )  GO TO 300
C 
      WRITE ( 6, 9100 )
 9100 FORMAT (1X, "DEBUG OUTPUT FOR FUNCTION DOTP." ) 
C 
      WRITE ( 6, 9200 )  A, B, DOTP
 9200 FORMAT (1X, "A   = ", 3 ( D30.16, 10X ), /, 1X, 
     1            'B   = ', 3 ( D30.16, 10X ), /, 1X, 
     2            'DOTP   = ', D30.16 ) 
C 
C   3.    NORMAL PROGRAM CONCLUSION.
C 
  300 RETURN
      END 
C******************************************************************************
      Real*8 FUNCTION VECMG ( A )
      IMPLICIT None
C 
C 4.    VECMG 
C 
C 4.1   VECMG PROGRAM SPECIFICATION 
C 
C 4.1.1 VECMG IS THE VECTOR UTILITY ROUTINE WHICH COMPUTES THE
C       MAGNITUDE OF A VECTOR.
C 
C 4.1.2 RESTRICTIONS - NONE 
C 
C 4.1.3 REFERENCES - NONE 
C 
C 4.2   VECMG PROGRAM INTERFACE 
C 
C 4.2.1 CALLING SEQUENCE - NONE 
C 
C           INPUT VARIABLES:
C             1.  A(3)  -  THE VECTOR IN QUESTION.
C 
C           OUTPUT VARIABLES: 
C             1.  VECMG  -  THE MAGNITUDE OF THE VECTOR A.
C 
C 4.2.2 COMMON BLOCKS USED -
C 
      INCLUDE 'ccon.i'               
C 
C            VARIABLES "FROM":
C              1.  KVECC  -  THE VECTOR UTILITY ROUTINE FLOW CONTROL FLAG.
C              2.  KVECD  -  THE VECTOR UTILITY ROUTINE DEBUG OUTPUT FLAG.
C 
C            VARIABLES "TO": NONE 
C 
C 4.2.3 PROGRAM SPECIFICATIONS -
C 
      Real*8 A(3), DOTP 
C 
C 4.2.4 DATA BASE ACCESS - NONE 
C 
C 4.2.5 EXTERNAL INPUT/OUTPUT -  POSSIBLE DEBUG OUTPUT
C 
C 4.2.6 SUBROUTINE INTERFACE -
C             CALLER SUBROUTINES: ETDG, SUNCR, et., etc. 
C             CALLED SUBROUTINES: DOTP, DSQRT 
C 
C 4.2.7 CONSTANTS USED - NONE 
C 
C 4.2.8 PROGRAM VARIABLES - NONE
C 
C 4.2.9 PROGRAMMER - DALE MARKHAM  01/19/77 
C                    PETER DENATALE 07/18/77
C                    David Gordon 94.04.18 Converted to Implicit None.
C 
C 4.3   VECMG PROGRAM STRUCTURE 
C 
C   1.    PERFORM THE COMPUTATION.
C 
      VECMG = DSQRT ( DOTP ( A, A ) ) 
C 
C   2.    CHECK KVECD TO DETERMINE IF DEBUG OUTPUT IS NECESSARY.
C 
      IF ( KVECD .EQ. 0 )  GO TO 300
C 
      WRITE ( 6, 9100 )
 9100 FORMAT (1X, "DEBUG OUTPUT FOR FUNCTION VECMG." )
C 
      WRITE ( 6, 9200 )  A, VECMG
 9200 FORMAT (1X, "A    = ", 3 ( D30.16, 10X ), /, 1X,
     1            'VECMG = ', D30.16 )
C 
C   3.    NORMAL PROGRAM CONCLUSION.
C 
  300 RETURN
      END 
C******************************************************************************
      SUBROUTINE VUNIT ( A, B )
      IMPLICIT None
C 
C 5.    VUNIT 
C 
C 5.1   VUNIT PROGRAM SPECIFICATION 
C 
C 5.1.1 VUNIT IS THE VECTOR UTILITY ROUTINE WHICH NORMALIZES A
C       VECTOR TO PRODUCE A UNIT VECTOR.
C 
C 5.1.2 RESTRICTIONS - NONE 
C 
C 5.1.3 REFERENCES - NONE 
C 
C 5.2   VUNIT PROGRAM INTERFACE 
C 
C 5.2.1 CALLING SEQUENCE - NONE 
C 
C           INPUT VARIABLES:
C             1.  A(3)  -  THE UNNORMALIZED VECTOR. 
C 
C           OUTPUT VARIABLES: 
C             1.  B(3)  -  THE NORMALIZED UNIT VECTOR.
C 
C 5.2.2 COMMON BLOCKS USED -
C 
      INCLUDE 'ccon.i'                 
C 
C            VARIABLES "FROM":
C              1.  KVECC  -  THE VECTOR UTILITY ROUTINE FLOW CONTROL FLAG.
C              2.  KVECD  -  THE VECTOR UTILITY ROUTINE DEBUG OUTPUT FLAG.
C 
C            VARIABLES "TO": NONE 
C 
C 5.2.3 PROGRAM SPECIFICATIONS -
C 
      Real*8 A(3), B(3), Amag, Vecmg 
      Integer*4 I
C 
C 5.2.4 DATA BASE ACCESS - NONE 
C 
C 5.2.5 EXTERNAL INPUT/OUTPUT - POSSIBLE DEBUG OUTPUT
C 
C 5.2.6 SUBROUTINE INTERFACE -
C             CALLER SUBROUTINES: STRCR, SUNCR, THERR     
C             CALLED SUBROUTINES: VECMG 
C 
C 5.2.7 CONSTANTS USED - NONE 
C 
C 5.2.8 PROGRAM VARIABLES - 
C           1.  AMAG  -  THE MAGNITUDE OF THE VECTOR A. 
C 
C 5.2.9 PROGRAMMER - DALE MARKHAM  01/19/77 
C                    PETER DENATALE 07/18/77
C                    David Gordon 94.04.18 Converted to Implicit None.
C 
C 5.3   VUNIT PROGRAM STRUCTURE 
C 
C   1.    PERFORM THE NORMALIZATION.
C 
      AMAG = VECMG ( A )
      DO 100  I = 1,3 
           B(I) = A(I) / AMAG 
  100 CONTINUE
C 
C   2.    CHECK KVECD TO DETERMINE IF DEBUG OUTPUT IS NECESSARY.
C 
      IF ( KVECD .EQ. 0 )  GO TO 300
C 
      WRITE ( 6, 9100 )
 9100 FORMAT (1X, "DEBUG OUTPUT FOR SUBROUTINE VUNIT." )
C 
      WRITE ( 6, 9200 )  A, B, AMAG
 9200 FORMAT (1X, "A    = ", 3 ( D30.16, 10X ), /, 1X,
     1            'B    = ', 3 ( D30.16, 10X ), /, 1X,
     2            'AMAG = ', D30.16 ) 
C 
C   3.    NORMAL PROGRAM CONCLUSION.
C 
  300 RETURN
      END 
C******************************************************************************
      SUBROUTINE CROSP ( A, B, C )
      IMPLICIT None
C 
C 6.    CROSP 
C 
C 6.1   CROSP PROGRAM SPECIFICATION 
C 
C 6.1.1 CROSP IS THE VECTOR UTILITY ROUTINE WHICH COMPUTES THE
C       CROSS PRODUCT OF TWO VECTORS. 
C 
C 6.1.2 RESTRICTIONS - NONE 
C 
C 6.1.3 REFERENCES - NONE 
C 
C 6.2   CROSP PROGRAM INTERFACE 
C 
C 6.2.1 CALLING SEQUENCE -
C 
C           INPUT VARIABLES:
C             1.  A(3)  -  THE FIRST VECTOR.
C             2.  B(3)  -  THE SECOND VECTOR. 
C 
C           OUTPUT VARIABLES: 
C             3.  C(3)  -  THE VECTOR REPRESENTING THE CROSS PRODUCT OF A x B.
C 
C 6.2.2 COMMON BLOCKS USED -
C 
      INCLUDE 'ccon.i'               
C 
C            VARIABLES "FROM":
C              1.  KVECC  -  THE VECTOR UTILITY ROUTINE FLOW CONTROL FLAG.
C              2.  KVECD  -  THE VECTOR UTILITY ROUTINE DEBUG OUTPUT FLAG.
C 
C            VARIABLES "TO": NONE 
C 
C 6.2.3 PROGRAM SPECIFICATIONS -
C 
      Real*8 A(3), B(3), C(3) 
C 
C 6.2.4 DATA BASE ACCESS - NONE 
C 
C 6.2.5 EXTERNAL INPUT/OUTPUT -  POSSIBLE DEBUG OUTPUT
C 
C 6.2.6 SUBROUTINE INTERFACE -
C             CALLER SUBROUTINES: SUNCR 
C             CALLED SUBROUTINES: NONE
C 
C 6.2.7 CONSTANTS USED - NONE 
C 
C 6.2.8 PROGRAM VARIABLES - NONE
C 
C 6.2.9 PROGRAMMER - DALE MARKHAM  01/19/77 
C                    PETER DENATALE 07/18/77
C                    David Gordon 94.04.18 Converted to Implicit None.
C 
C 6.3   CROSP PROGRAM STRUCTURE 
C 
C   1.    PERFORM THE CROSS PRODUCT.
C 
      C(1) = A(2) * B(3)  -  B(2) * A(3)
      C(2) = A(3) * B(1)  -  B(3) * A(1)
      C(3) = A(1) * B(2)  -  B(1) * A(2)
C 
C   2.    CHECK KVECD TO DETERMINE IF DEBUG OUTPUT IS NECESSARY.
C 
      IF ( KVECD .EQ. 0 )  GO TO 300
C 
      WRITE ( 6, 9100 )
 9100 FORMAT (1X, "DEBUG OUTPUT FOR SUBROUTINE CROSP." )
C 
      WRITE ( 6, 9200 )  A, B, C 
 9200 FORMAT (1X, "A    = ", 3 ( D30.16, 10X ), /, 1X,
     1            'B    = ', 3 ( D30.16, 10X ), /, 1X,
     2            'C    = ', 3 ( D30.16, 10X ) )
C 
C   3.    NORMAL PROGRAM CONCLUSION.
C 
  300 RETURN
      END 