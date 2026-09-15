# 🌅 Chapter 2: Morning Quick Memorization Guide
> **Vector Spaces, Subspaces, Rank, Basis & The Four Fundamental Subspaces**  
> *Estimated reading time: 3 minutes*

---

## 1. What is a Subspace?
A subset $S$ of $\mathbb{R}^n$ is a subspace if:
1. **Contains $\mathbf{0}$:** The zero vector must be in $S$.
2. **Closed under addition:** If $u, v \in S \implies u + v \in S$.
3. **Closed under scalar multiplication:** If $u \in S, c \in \mathbb{R} \implies c u \in S$.
> **Exam Quick Check:** Does it pass through the origin $(0,0,\dots,0)$? If not, it is **NEVER** a subspace! (e.g., $x + y = 1$ is NOT a subspace).

---

## 2. Column Space $C(A)$ vs. Nullspace $N(A)$

| Feature | Column Space $C(A)$ | Nullspace $N(A)$ |
| :--- | :--- | :--- |
| **Definition** | All combinations of columns of $A$ | All solutions $x$ to $Ax = \mathbf{0}$ |
| **Equation** | Outputs $b$ such that $Ax = b$ | Inputs $x$ that produce $Ax = \mathbf{0}$ |
| **Lives in** | $\mathbf{\mathbb{R}^m}$ ($m$ rows) | $\mathbf{\mathbb{R}^n}$ ($n$ columns) |
| **Dimension** | $\mathbf{r}$ (Rank = number of pivots) | $\mathbf{n - r}$ (Number of free variables) |
| **Solvability** | $Ax = b$ has a solution $\iff b \in C(A)$ | $Ax = 0$ always has trivial solution $x = \mathbf{0}$ |

---

## 3. Rank, Pivots & Free Variables
- **Rank ($r$):** The number of non-zero pivots after elimination.
- **Pivot Variables ($r$):** Variables corresponding to columns with pivots.
- **Free Variables ($n - r$):** Variables corresponding to columns WITHOUT pivots.
- **Special Solutions to $Ax = 0$:**
  - Set one free variable $= 1$ and all other free variables $= 0$.
  - Solve for the pivot variables via back-substitution.
  - Number of special solutions = $n - r =$ basis vectors for $N(A)$.

---

## 4. Complete Solution to $Ax = b$
$$\mathbf{x = x_p + x_n}$$
- **$x_p$ (Particular solution):** Set all free variables to $0$, solve for pivot variables in $Ux = c$.
- **$x_n$ (Nullspace solution):** Any linear combination of special solutions: $c_1 s_1 + c_2 s_2 + \dots$
- If $r = n$ (no free variables), the solution is **unique** (if it exists).

---

## 5. Independence, Span, Basis & Dimension
- **Linearly Independent:** $c_1 v_1 + \dots + c_k v_k = \mathbf{0}$ only when all $c_i = 0$.
  - Columns of $A$ are independent $\iff N(A) = \{\mathbf{0}\} \iff r = n$.
- **Span:** The set of all linear combinations of the vectors.
- **Basis:** A set of vectors that is:
  1. Linearly Independent, **AND**
  2. Spans the space.
  *(A minimal spanning set / maximal independent set).*
- **Dimension:** The number of vectors in a basis.
  - $\dim C(A) = r$
  - $\dim N(A) = n - r$
- **Rank-Nullity Theorem:**
  $$\mathbf{\dim C(A) + \dim N(A) = n} \quad (r + (n - r) = n)$$

---

## 6. The Four Fundamental Subspaces (BIG PICTURE ⭐)
For any $m \times n$ matrix $A$ with rank $r$:

| Subspace | Notation | Lives In | Dimension | How to Find Basis |
| :--- | :---: | :---: | :---: | :--- |
| **Column Space** | $C(A)$ | $\mathbb{R}^m$ | $\mathbf{r}$ | Pivot columns of **original** matrix $A$ |
| **Nullspace** | $N(A)$ | $\mathbb{R}^n$ | $\mathbf{n - r}$ | Special solutions of $Ax = \mathbf{0}$ |
| **Row Space** | $C(A^T)$ | $\mathbb{R}^n$ | $\mathbf{r}$ | Non-zero rows of echelon matrix $U$ (or $R$) |
| **Left Nullspace** | $N(A^T)$ | $\mathbb{R}^m$ | $\mathbf{m - r}$ | Special solutions of $A^T y = \mathbf{0}$ |

> **Key Symmetry:** $\dim(\text{Column Space}) = \dim(\text{Row Space}) = r$.  
> Number of independent columns always equals number of independent rows!

---

## 7. The 4 Rank Possibilities at a Glance
- **Full Column Rank ($r = n$):**
  - No free variables $\implies N(A) = \{\mathbf{0}\}$ (dim 0).
  - Columns are independent.
  - $Ax = b$ has **$0$ or $1$ solution**.
- **Full Row Rank ($r = m$):**
  - Rows are independent.
  - Column space is all of $\mathbb{R}^m$ ($C(A) = \mathbb{R}^m$).
  - $Ax = b$ has **at least $1$ solution for EVERY $b$** ($1$ or $\infty$ solutions).
- **Full Rank Square ($r = m = n$):**
  - Invertible matrix!
  - $Ax = b$ has **exactly $1$ unique solution** $x = A^{-1}b$ for every $b$.
  - $N(A) = \{\mathbf{0}\}$.
- **Incomplete Rank ($r < m$ and $r < n$):**
  - Solutions exist only if $b \in C(A)$; if so, infinitely many solutions.

---

## ⚠️ Chapter 2 Morning Traps to Avoid
1. **Basis for $C(A)$:** Pick the pivot columns from the **ORIGINAL matrix $A$**, NOT from the reduced matrix $U$!
2. **Basis for Row Space $C(A^T)$:** Pick the non-zero rows from the **reduced matrix $U$** (or $A$); row operations do NOT change row space!
3. **Dimensions:** $N(A)$ lives in $\mathbb{R}^n$ (dimension $n - r$); $C(A)$ lives in $\mathbb{R}^m$ (dimension $r$). Don't mix $m$ and $n$!
