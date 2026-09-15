# 🌅 Chapter 1: Morning Quick Memorization Guide
> **Linear Systems, Matrices, Gaussian Elimination & LU Factorization**  
> *Estimated reading time: 3 minutes*

---

## 1. The Core Setup: $Ax = b$
- $A$: $m \times n$ coefficient matrix ($m$ equations, $n$ unknowns).
- $x$: $n \times 1$ vector of unknowns.
- $b$: $m \times 1$ right-hand side vector.

**Row vs. Column Picture:**
- **Row Picture:** Each equation is a line (2D) or plane (3D). Solution is the intersection point.
- **Column Picture:** Expresses $b$ as a **linear combination of columns**:
  $$u \begin{bmatrix} a_{11} \\ a_{21} \\ a_{31} \end{bmatrix} + v \begin{bmatrix} a_{12} \\ a_{22} \\ a_{32} \end{bmatrix} + w \begin{bmatrix} a_{13} \\ a_{23} \\ a_{33} \end{bmatrix} = \begin{bmatrix} b_1 \\ b_2 \\ b_3 \end{bmatrix}$$

---

## 2. Gaussian Elimination in 3 Steps
1. **Pivots:** The first non-zero entry on the diagonal of each row.
   - **Rule:** A pivot can NEVER be zero! If zero, swap rows. If the rest of the column is zero, the matrix is **singular**.
2. **Multipliers:**
   $$l_{ij} = \frac{\text{entry to eliminate in Row } i}{\text{pivot entry in Row } j}$$
   Operation: $\text{Row } i \leftarrow \text{Row } i - l_{ij} \times \text{Row } j$.
3. **Back-Substitution:**
   - Bottom equation has 1 unknown $\implies$ solve it first.
   - Substitute upward row by row to find all unknowns.

---

## 3. Matrix Multiplication Quick Rules
- $(m \times k) \times (k \times n) \implies (m \times n)$.
- Entry $(i, j)$ of $AB = (\text{Row } i \text{ of } A) \cdot (\text{Column } j \text{ of } B)$.
- **Non-commutative:** In general, $AB \ne BA$.

---

## 4. $LU$ Factorization: $A = LU$
- **$U$ (Upper Triangular):** The matrix obtained immediately after forward elimination. All entries below diagonal are $0$.
- **$L$ (Lower Triangular):** 
  - Diagonal entries are all **$1$**.
  - All entries above diagonal are $0$.
  - Entries below diagonal are the exact **elimination multipliers $l_{ij}$** used in Gaussian elimination.
- **Why $LU$?** To solve $Ax = b$ fast for multiple $b$:
  1. Solve $Lc = b$ (Forward substitution: top to bottom).
  2. Solve $Ux = c$ (Back substitution: bottom to top).
- **With row swaps:** $PA = LU$ ($P$ is permutation matrix).

---

## 5. Matrix Inverses ($A^{-1}$)
- Defined only for **square matrices** ($n \times n$).
- $A A^{-1} = A^{-1} A = I$.
- **Invertible $\iff$ Non-singular $\iff n$ non-zero pivots $\iff \det(A) \ne 0 \iff Ax = 0$ has only $x = 0$**.
- **$2 \times 2$ Inverse Shortcut:**
  $$\begin{bmatrix} a & b \\ c & d \end{bmatrix}^{-1} = \frac{1}{ad - bc}\begin{bmatrix} d & -b \\ -c & a \end{bmatrix}$$
- **Gauss-Jordan Method:**
  $$[A \mid I] \xrightarrow{\text{elimination steps}} [I \mid A^{-1}]$$

---

## 6. Transposes & Symmetric Matrices
- **Transpose ($A^T$):** Flip rows and columns: $(A^T)_{ij} = A_{ji}$.
- **Reversal Rule:** 
  $$\mathbf{(AB)^T = B^T A^T} \quad \text{and} \quad \mathbf{(AB)^{-1} = B^{-1} A^{-1}}$$
- **Symmetric Matrix:** $A^T = A$ (always square).
- **Universal Symmetry Property:**
  For *any* matrix $R$ (even rectangular):
  $$\mathbf{R^T R} \quad \text{and} \quad \mathbf{R R^T} \quad \text{are ALWAYS symmetric!}$$
  *(Proof: $(R^TR)^T = R^T(R^T)^T = R^TR$)*.

---

## ⚠️ Chapter 1 Morning Traps to Avoid
1. **Never forget the minus sign:** Operation is $\text{Row}_2 - l_{21}\text{Row}_1$, but the multiplier $l_{21}$ enters into $L$ with its positive sign!
2. **Order matters:** $(AB)^{-1} \ne A^{-1}B^{-1}$; it is $B^{-1}A^{-1}$.
3. **Singular matrix:** Zero in pivot position that cannot be eliminated by row exchange $\implies$ NO inverse exists.
