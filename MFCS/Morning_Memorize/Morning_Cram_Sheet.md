# ⚡ 5-Minute Morning Cram Sheet: Chapters 1, 2 & 3
> **Quick Memorization Card for Exam Morning**  
> *Read this right before your exam or right when you wake up. Everything here is pure high-yield testable material.*

---

## 🚀 Part 1: Quick Formula & Dimension Reference

| Concept | Symbol | Lives In | Dimension / Value | Key Meaning |
| :--- | :---: | :---: | :---: | :--- |
| **Matrix Size** | $A$ | - | $m \times n$ | $m$ rows (equations), $n$ columns (unknowns) |
| **Rank** | $r$ | - | Number of pivots | Number of independent rows & columns ($r \le \min(m, n)$) |
| **Column Space** | $C(A)$ | $\mathbb{R}^m$ | $\mathbf{r}$ | All outputs $Ax$; $Ax=b$ solvable $\iff b \in C(A)$ |
| **Nullspace** | $N(A)$ | $\mathbb{R}^n$ | $\mathbf{n - r}$ | All solutions to $Ax = \mathbf{0}$ |
| **Row Space** | $C(A^T)$ | $\mathbb{R}^n$ | $\mathbf{r}$ | All combinations of rows of $A$ |
| **Left Nullspace** | $N(A^T)$ | $\mathbb{R}^m$ | $\mathbf{m - r}$ | All solutions to $A^T y = \mathbf{0}$ |
| **Free Variables** | - | - | $\mathbf{n - r}$ | Columns without pivots |
| **Rank-Nullity** | - | - | $r + (n - r) = n$ | $\dim C(A) + \dim N(A) = \text{number of columns}$ |
| **Orthogonal Complements** | $C(A^T) \perp N(A)$ | $\mathbb{R}^n$ | $r + (n - r) = n$ | Row Space $\perp$ Nullspace |
| **Left Complements** | $C(A) \perp N(A^T)$ | $\mathbb{R}^m$ | $r + (m - r) = m$ | Column Space $\perp$ Left Nullspace |

---

## 🧠 CHAPTER 1: Core Essentials to Memorize

### 1. The 3 Views of $Ax = b$
- **Row Picture:** Intersection of hyperplanes/lines.
- **Column Picture:** Linear combination of columns: $x_1 a_1 + x_2 a_2 + \dots + x_n a_n = b$.
- **Matrix Picture:** Linear transformation mapping $x \in \mathbb{R}^n \to b \in \mathbb{R}^m$.

### 2. Gaussian Elimination & Pivots
- **Forward Elimination:** Converts $A \to U$ (Upper Triangular).
- **Pivot:** The first non-zero entry in a row. **Never zero!**
  - If pivot is $0$, swap with a lower row.
  - If no non-zero entry below, the matrix is **singular** (fewer than $n$ pivots).
- **Back-Substitution:** Solve from bottom row upwards ($z \to y \to x$).

### 3. Matrix Inverses ($A^{-1}$)
- Exists **if and only if** $A$ is square ($n \times n$) and has **$n$ pivots** ($r = n$, non-singular, $\det(A) \ne 0$).
- If $A^{-1}$ exists, $Ax = b$ has the **unique solution** $x = A^{-1}b$.
- **$2 \times 2$ Inverse Formula:**
  $$\begin{bmatrix} a & b \\ c & d \end{bmatrix}^{-1} = \frac{1}{ad - bc}\begin{bmatrix} d & -b \\ -c & a \end{bmatrix}$$
- **Gauss-Jordan Method:** Row reduce $[A \mid I] \longrightarrow [I \mid A^{-1}]$.

### 4. $LU$ Factorization ($A = LU$)
- $U$ = Upper triangular matrix (from elimination).
- $L$ = Lower triangular matrix with **$1$s on the diagonal** and **multipliers $l_{ij}$ below**.
- If row exchanges were needed: $PA = LU$ ($P$ is permutation matrix).
- **Solving $Ax = b$:**
  1. Solve $Lc = b$ by forward substitution.
  2. Solve $Ux = c$ by back substitution.

### 5. Transpose & Symmetry Rules *(High-Yield Traps!)*
- $(AB)^T = B^T A^T$ and $(AB)^{-1} = B^{-1} A^{-1}$ **(Always reverse order!)**
- $(A^T)^{-1} = (A^{-1})^T$
- **Symmetric Matrix:** $A^T = A$ (must be square).
- **Golden Rule:** For ANY matrix $R$ ($m \times n$), **$R^T R$ and $R R^T$ are ALWAYS symmetric!**
  - Proof: $(R^T R)^T = R^T (R^T)^T = R^T R$.

---

## 🧠 CHAPTER 2: Core Essentials to Memorize

### 1. Vector Space & Subspace Requirements
- Must satisfy **closure**:
  1. $x + y \in S$ (Closed under addition)
  2. $c x \in S$ (Closed under scalar multiplication)
- **Golden Test:** Every subspace **MUST contain the zero vector $\mathbf{0}$!**
  - Line/plane passing through $(0,0,0)$ $\implies$ **Subspace**.
  - Line/plane NOT passing through origin ($x + y = 1$) $\implies$ **NOT a subspace**.

### 2. Column Space $C(A)$ vs Nullspace $N(A)$
- **$C(A)$:** Span of columns of $A$. Lives in $\mathbb{R}^m$.
  - $Ax = b$ is solvable **$\iff b \in C(A)$**.
- **$N(A)$:** All solutions to $Ax = \mathbf{0}$. Lives in $\mathbb{R}^n$.
  - Found by setting each free variable to $1$ and others to $0$ $\implies$ gives $n - r$ special solutions.

### 3. Complete Solution to $Ax = b$
$$x = x_{\text{particular}} + x_{\text{nullspace}} = x_p + c_1 s_1 + c_2 s_2 + \dots$$
- **$x_p$:** Set all free variables $= 0$, solve for pivot variables.
- **$x_n$:** Linear combinations of special solutions to $Ax = \mathbf{0}$.

### 4. Independence, Span, Basis, Dimension
- **Linearly Independent:** $c_1 v_1 + \dots + c_k v_k = \mathbf{0} \implies$ only $c_1 = \dots = c_k = 0$.
  - Columns of $A$ are independent $\iff N(A) = \{\mathbf{0}\} \iff \text{rank } r = n$.
- **Span:** The set of ALL possible linear combinations of the vectors.
- **Basis:** A set of vectors that is **both** (1) Linearly Independent and (2) Spans the space.
- **Dimension:** The **number of vectors** in any basis for that space.

### 5. The 4 Rank Cases for $Ax = b$
| Rank | Size Relation | Nullspace $N(A)$ | Number of Solutions to $Ax = b$ | Description |
| :--- | :--- | :--- | :--- | :--- |
| **$r = m = n$** | Square | $\{\mathbf{0}\}$ (dim 0) | **Exactly 1 unique solution** | Invertible, full rank |
| **$r = n < m$** | Tall / Skinny | $\{\mathbf{0}\}$ (dim 0) | **0 or 1 unique solution** | Full column rank, independent cols |
| **$r = m < n$** | Short / Wide | $\dim = n - m > 0$ | **Infinitely many solutions** | Full row rank, solvable for any $b$ |
| **$r < m$ and $r < n$** | Any shape | $\dim = n - r > 0$ | **0 or Infinitely many** | Not full rank |

---

## 🧠 CHAPTER 3: Core Essentials to Memorize

### 1. Orthogonality Condition & Subspace Complements
- $x \perp y \iff \mathbf{x^T y = 0}$.
- Fundamental Orthogonality: $\mathbf{C(A^T) \perp N(A)}$ (in $\mathbb{R}^n$) and $\mathbf{C(A) \perp N(A^T)}$ (in $\mathbb{R}^m$).

### 2. Projections & Projection Matrices
- **Line projection:** $p = a \frac{a^Tb}{a^Ta}$, with projection matrix $\mathbf{P = \frac{aa^T}{a^Ta}}$.
- **General projection onto $C(A)$:** $\mathbf{P = A(A^TA)^{-1}A^T}$ and $p = Pb$.
- **Two Invariant Properties of ANY $P$:**
  1. $\mathbf{P^T = P}$ (Symmetric)
  2. $\mathbf{P^2 = P}$ (Idempotent)

### 3. Least Squares & The Normal Equations
- When $Ax = b$ is unsolvable ($b \notin C(A)$): minimize $\|Ax - b\|^2$.
- Solve the **Normal Equations:**
  $$\mathbf{A^T A \hat{x} = A^T b} \implies \hat{x} = (A^T A)^{-1} A^T b$$
- Projection: $p = A\hat{x}$; Error: $e = b - p \in N(A^T)$ (strictly $A^Te = \mathbf{0}$).

### 4. Orthonormal Matrices ($Q$) & Gram-Schmidt
- $Q^TQ = I$. If $Q$ is square: $\mathbf{Q^{-1} = Q^T}$.
- Projection matrix with orthonormal columns: $\mathbf{P = QQ^T}$.
- **Gram-Schmidt:** Subtract parallel projections onto earlier directions, then divide by length:
  $$A = a, \quad B = b - \frac{A^Tb}{A^TA}A, \quad C = c - \frac{A^Tc}{A^TA}A - \frac{B^Tc}{B^TB}B \implies q_i = \frac{V_i}{\|V_i\|}$$

---

## 🎯 5 One-Line Golden Rules for the Exam

1. **Rank = Pivots = $\dim C(A) = \dim C(A^T)$.**
2. **Free Variables = $n - r = \dim N(A)$.**
3. **$\dim(\text{Column Space}) + \dim(\text{Nullspace}) = n$ (Total Columns).**
4. **$C(A^T) \perp N(A)$ and $C(A) \perp N(A^T)$ (Orthogonal Complements).**
5. **Least-squares always solves $A^TA\hat{x} = A^Tb$ with perpendicular error $A^Te = \mathbf{0}$.**

