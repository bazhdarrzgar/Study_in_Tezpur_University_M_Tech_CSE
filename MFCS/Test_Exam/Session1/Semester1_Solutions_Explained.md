# TEZPUR UNIVERSITY
## Autumn Semester Test I Examination, 2026
**Course:** CSMT 500 / CSMT 530  
**Date:** 15/09/2026 | **Full Marks:** 10

---

## Q1 (1 Mark)

> For four linear equations in two unknowns $x$ and $y$, the row picture shows four **lines**. The column picture is in **four-dimensional** space. The equations have no solution unless the vector on the right-hand side is a combination of **the two column vectors**.

### Why this answer is correct

**Blank 1 — "lines":**  
The system has the form $Ax = b$ where $A$ is $4 \times 2$ and $x \in \mathbb{R}^2$. Each of the 4 equations $a_{i1}x + a_{i2}y = b_i$ describes one **straight line** in the 2D $xy$-plane. Because there are 4 equations, the row picture shows **four lines**.

**Blank 2 — "four-dimensional":**  
Rewriting as $x\,\mathbf{c}_1 + y\,\mathbf{c}_2 = \mathbf{b}$, each column of $A$ has **4 components** (one per equation). So the column vectors live in $\mathbb{R}^4$, making the column picture **four-dimensional**.

**Blank 3 — "the two column vectors":**  
The system $Ax = b$ is solvable **if and only if** $b$ lies in the column space $C(A)$. Here $A$ has only 2 columns, so $C(A) = \text{span}\{\mathbf{c}_1, \mathbf{c}_2\}$. A solution exists only when $\mathbf{b}$ is a linear combination of those two column vectors.

---

## Q2 (1 Mark)

**Test:** $b_2 = 2b_1$

**Number of solutions:**
- If $b_2 \neq 2b_1$ → **No solution**
- If $b_2 = 2b_1$ → **Infinitely many solutions**

**Column picture:**

$$x \begin{bmatrix} 3 \\ 6 \end{bmatrix} + y \begin{bmatrix} -2 \\ -4 \end{bmatrix} = \begin{bmatrix} b_1 \\ b_2 \end{bmatrix}$$

Both columns are collinear (on the same line $b_2 = 2b_1$ in $\mathbb{R}^2$):

```
     b2 ^
        |       . (3,6)
        |     .
        |   .
        | .
        +-----------> b1
      .  |
    .    |
(-2,-4)  |
```

### Why this answer is correct

**Why the test is $b_2 = 2b_1$:**  
Multiply the first equation $3x - 2y = b_1$ by $2$:
$$6x - 4y = 2b_1$$
This is identical to the left side of equation 2. For consistency, the right-hand sides must also match, so we need $b_2 = 2b_1$.

**Why no solution when $b_2 \neq 2b_1$:**  
The two equations represent **parallel lines** (same slope, different intercepts). Parallel lines never meet, so there is no solution.

**Why infinitely many solutions when $b_2 = 2b_1$:**  
The two equations become identical (the same line). Every point on that line is a solution. Since the line has one free variable ($y$ can be any real number), there are **infinitely many** solutions.

**Why the column picture shows a single line:**  
Both columns $\begin{bmatrix}3\\6\end{bmatrix}$ and $\begin{bmatrix}-2\\-4\end{bmatrix}$ are scalar multiples of each other:
$$\begin{bmatrix}-2\\-4\end{bmatrix} = -\tfrac{2}{3}\begin{bmatrix}3\\6\end{bmatrix}$$
They are **linearly dependent** — they point along the same direction. Any combination $x\mathbf{c}_1 + y\mathbf{c}_2$ can only reach vectors **on the line** $b_2 = 2b_1$. If $\mathbf{b}$ is off this line, it is unreachable (no solution).

---

## Q3 (1 Mark)

$$\boxed{P = \begin{bmatrix} 0 & 1 & 0 \\ 0 & 0 & 1 \\ 1 & 0 & 0 \end{bmatrix}}$$

### Why this answer is correct

A **permutation matrix** is built by reordering the rows of the identity matrix $I$. Multiplying $PA$ on the left applies the row permutation to $A$.

**Step 1 — Swap rows 1 and 2:**
$$P_{12} = \begin{bmatrix}0&1&0\\1&0&0\\0&0&1\end{bmatrix} \implies P_{12}A = \begin{bmatrix}\mathbf{r}_2\\\mathbf{r}_1\\\mathbf{r}_3\end{bmatrix}$$

**Step 2 — Then swap rows 2 and 3 of the result:**
$$P_{23} = \begin{bmatrix}1&0&0\\0&0&1\\0&1&0\end{bmatrix} \implies P_{23}(P_{12}A) = \begin{bmatrix}\mathbf{r}_2\\\mathbf{r}_3\\\mathbf{r}_1\end{bmatrix}$$

**Combined matrix $P = P_{23}P_{12}$:**
$$P = \begin{bmatrix}1&0&0\\0&0&1\\0&1&0\end{bmatrix}\begin{bmatrix}0&1&0\\1&0&0\\0&0&1\end{bmatrix} = \begin{bmatrix}0&1&0\\0&0&1\\1&0&0\end{bmatrix}$$

**Verification:**
- Row 1 of $PA$ = Row 2 of $A$ ✓
- Row 2 of $PA$ = Row 3 of $A$ ✓
- Row 3 of $PA$ = Row 1 of $A$ ✓

This is the unique $3 \times 3$ permutation matrix that performs both swaps in sequence.

---

## Q4 (2 Marks)

The correct descriptions are: **(b), (d), (e)**

- **(b) a line**
- **(d) a subspace**
- **(e) the nullspace of A**

### Why this answer is correct

**Solving $Ax = 0$:**
$$\begin{cases} x_1 + x_2 + x_3 = 0 \\ x_1 + 2x_3 = 0 \end{cases}$$

From equation (2): $x_1 = -2x_3$. Substituting into (1): $x_2 = x_3$.  
Letting $x_3 = t$:
$$\mathbf{x} = t\begin{bmatrix}-2\\1\\1\end{bmatrix}, \quad t \in \mathbb{R}$$

**Why (b) is correct — "a line":**  
Rank $r = 2$ (two pivot columns), so $\dim(N(A)) = n - r = 3 - 2 = 1$.  
A 1-dimensional subspace through the origin in $\mathbb{R}^3$ is exactly a **straight line**.

**Why (d) is correct — "a subspace":**  
The solution set of any homogeneous system $Ax = \mathbf{0}$ is always a subspace: it contains the zero vector, and is closed under addition and scalar multiplication.

**Why (e) is correct — "the nullspace of A":**  
By definition, $N(A) = \{x \in \mathbb{R}^3 : Ax = \mathbf{0}\}$, which is exactly what we computed.

**Why (a) is wrong — "a plane":**  
A plane is 2-dimensional. Here $\dim(N(A)) = 1$, so the solution set is 1-dimensional — a line, not a plane.

**Why (c) is wrong — "a point":**  
A point would mean only $x = \mathbf{0}$ satisfies $Ax = \mathbf{0}$, i.e., $N(A) = \{\mathbf{0}\}$. But we found infinitely many solutions.

**Why (f) is wrong — "the column space of A":**  
$C(A) \subseteq \mathbb{R}^2$ (the output space, since $A$ has 2 rows), while the solution vectors $x \in \mathbb{R}^3$ (the input space). They live in completely different spaces.

---

## Q5 (3 Marks)

For $A = \begin{bmatrix} 0 & 1 & 0 & 3 \\ 0 & 2 & 0 & 6 \end{bmatrix}$, $\text{rank} = 1$:

| Subspace | Notation | Dim | Basis |
|:---|:---:|:---:|:---|
| Column Space | $C(A)$ | $1$ | $\left\{\begin{bmatrix}1\\2\end{bmatrix}\right\}$ |
| Row Space | $C(A^T)$ | $1$ | $\left\{\begin{bmatrix}0\\1\\0\\3\end{bmatrix}\right\}$ |
| Nullspace | $N(A)$ | $3$ | $\left\{\begin{bmatrix}1\\0\\0\\0\end{bmatrix}, \begin{bmatrix}0\\0\\1\\0\end{bmatrix}, \begin{bmatrix}0\\-3\\0\\1\end{bmatrix}\right\}$ |
| Left Nullspace | $N(A^T)$ | $1$ | $\left\{\begin{bmatrix}-2\\1\end{bmatrix}\right\}$ |

### Why this answer is correct

**Step 1 — Row reduce to find the rank:**
$$R_2 \leftarrow R_2 - 2R_1 \implies \text{rref}(A) = \begin{bmatrix}0&1&0&3\\0&0&0&0\end{bmatrix}$$
There is **1 pivot** (in column 2), so $r = 1$. Free columns: 1, 3, 4.

**Column Space $C(A)$ — why $\dim = 1$, basis $\{[1,2]^T\}$:**  
$C(A)$ = span of the columns of $A$ = subspace of $\mathbb{R}^m = \mathbb{R}^2$.  
Only column 2 (the pivot column) gives an independent contribution: $\begin{bmatrix}1\\2\end{bmatrix}$.  
All other columns are multiples of it ($\text{col}_4 = 3 \cdot \text{col}_2$), so $\dim(C(A)) = r = 1$.

**Row Space $C(A^T)$ — why $\dim = 1$, basis $\{[0,1,0,3]^T\}$:**  
The row space = span of the non-zero rows of the RREF = $\begin{bmatrix}0&1&0&3\end{bmatrix}$. Dimension = $r = 1$.

**Nullspace $N(A)$ — why $\dim = 3$, with 3 special solutions:**  
$\dim(N(A)) = n - r = 4 - 1 = 3$. From $Ax = \mathbf{0}$: the single equation gives $x_2 = -3x_4$. Variables $x_1, x_3, x_4$ are free. Setting each free variable to 1 (others 0):
$$\mathbf{s}_1 = \begin{bmatrix}1\\0\\0\\0\end{bmatrix}, \quad \mathbf{s}_2 = \begin{bmatrix}0\\0\\1\\0\end{bmatrix}, \quad \mathbf{s}_3 = \begin{bmatrix}0\\-3\\0\\1\end{bmatrix}$$

**Left Nullspace $N(A^T)$ — why $\dim = 1$, basis $\{[-2,1]^T\}$:**  
$\dim(N(A^T)) = m - r = 2 - 1 = 1$. Solving $y^T A = 0^T$:
$$y_1 + 2y_2 = 0 \implies y_1 = -2y_2 \implies \mathbf{y} = y_2\begin{bmatrix}-2\\1\end{bmatrix}$$
**Orthogonality check:** $C(A) \perp N(A^T)$ since $\begin{bmatrix}1\\2\end{bmatrix}\cdot\begin{bmatrix}-2\\1\end{bmatrix} = -2+2 = 0$ ✓

---

## Q6 (2 Marks)

$$\boxed{A = \begin{bmatrix} 1 & 0 & -2 & -3 \\ 0 & 1 & -2 & -1 \end{bmatrix}}$$

### Why this answer is correct

**Step 1 — Determine the size and rank of $A$:**  
The null vectors $\mathbf{s}_1 = (2,2,1,0)^T$ and $\mathbf{s}_2 = (3,1,0,1)^T$ live in $\mathbb{R}^4$, so $A$ must have $n = 4$ columns.  
The identity block in components 3 and 4 ($\begin{bmatrix}1&0\\0&1\end{bmatrix}$) shows $x_3, x_4$ are **free variables** and $x_1, x_2$ are **pivot variables**.  
By the Rank-Nullity Theorem: $\text{rank} = n - \dim(N(A)) = 4 - 2 = 2$.

**Step 2 — Reconstruct the RREF block structure:**  
For a matrix with pivots in columns 1 and 2, the RREF has the form $R = [I \mid F]$.  
The nullspace matrix $[{-F}; I]$ equals the given null vectors, so:
$$-F = \begin{bmatrix}2&3\\2&1\end{bmatrix} \implies F = \begin{bmatrix}-2&-3\\-2&-1\end{bmatrix}$$
Therefore:
$$A = \begin{bmatrix}1&0&-2&-3\\0&1&-2&-1\end{bmatrix}$$

**Step 3 — Verification (rows must be orthogonal to both null vectors):**

$$A\,\mathbf{s}_1 = \begin{bmatrix}1(2)+0(2)-2(1)-3(0)\\0(2)+1(2)-2(1)-1(0)\end{bmatrix} = \begin{bmatrix}2-2\\2-2\end{bmatrix} = \begin{bmatrix}0\\0\end{bmatrix} \checkmark$$

$$A\,\mathbf{s}_2 = \begin{bmatrix}1(3)+0(1)-2(0)-3(1)\\0(3)+1(1)-2(0)-1(1)\end{bmatrix} = \begin{bmatrix}3-3\\1-1\end{bmatrix} = \begin{bmatrix}0\\0\end{bmatrix} \checkmark$$

Both null vectors are in $N(A)$, they are linearly independent, and $\dim(N(A)) = 4 - 2 = 2$, so they form a **complete basis** for the nullspace.
