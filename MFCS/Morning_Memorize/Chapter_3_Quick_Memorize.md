# 🌅 Chapter 3: Morning Quick Memorization Guide
> **Orthogonality, Projections, Least Squares & Gram-Schmidt**  
> *Estimated reading time: 3-4 minutes*

---

## 1. Length, Dot Product & Orthogonality
- **Vector Length (Norm):**
  $$\|x\| = \sqrt{x^T x} = \sqrt{x_1^2 + x_2^2 + \dots + x_n^2}$$
  - Note: $x^T x = \|x\|^2$ (Length squared).
- **Dot Product (Inner Product):**
  $$x^T y = x_1 y_1 + x_2 y_2 + \dots + x_n y_n$$
- **Orthogonality Condition:**
  $$\mathbf{x \perp y \iff x^T y = 0}$$
- **Pythagorean Theorem:** If $x \perp y$, then $\|x + y\|^2 = \|x\|^2 + \|y\|^2$.
- **Zero Vector:** $\mathbf{0}$ is orthogonal to **every** vector ($\mathbf{0}^T y = 0$).

---

## 2. Fundamental Orthogonality of the Subspaces ⭐⭐⭐
The Four Fundamental Subspaces pair up into **orthogonal complements**:

| Input Space $\mathbb{R}^n$ | Output Space $\mathbb{R}^m$ |
| :--- | :--- |
| $$\mathbf{C(A^T) \perp N(A)}$$ | $$\mathbf{C(A) \perp N(A^T)}$$ |
| **Row Space $\perp$ Nullspace** | **Column Space $\perp$ Left Nullspace** |
| $\dim = r + (n - r) = n$ | $\dim = r + (m - r) = m$ |
| Intersect **only** at $\mathbf{0}$ | Intersect **only** at $\mathbf{0}$ |

> **Why $C(A^T) \perp N(A)$?** Every row of $A$ multiplies $x \in N(A)$ to give $0$ ($Ax = \mathbf{0}$). Thus every vector in the row space is perpendicular to every vector in the nullspace!

---

## 3. Projection onto a Line (1D Subspace)
To project vector $b$ onto the line along vector $a$:

$$\mathbf{p = \hat{x} a = a \frac{a^T b}{a^T a}}$$

- **Scalar factor:** $\hat{x} = \frac{a^T b}{a^T a} = \frac{\text{dot product}}{\text{length squared of } a}$.
- **Error vector:** $e = b - p = b - \hat{x}a$.
- **Crucial geometric property:** Error is perpendicular to the line:
  $$a^T e = 0 \iff a^T (b - \hat{x}a) = 0$$
- **1D Projection Matrix:**
  $$\mathbf{P = \frac{a a^T}{a^T a}} \quad \implies \quad p = P b$$
  *(Note: $a a^T$ is an $n \times n$ matrix, $a^T a$ is a scalar).*

---

## 4. Projection onto a Subspace $C(A)$
When projecting $b$ onto the column space of matrix $A$ ($m \times n$, rank $r = n$):

$$\mathbf{p = A \hat{x} = A(A^T A)^{-1} A^T b}$$

$$\mathbf{P = A (A^T A)^{-1} A^T} \quad \implies \quad p = P b$$

### 2 Vital Properties of ANY Projection Matrix $P$:
1. **Symmetric:** $\mathbf{P^T = P}$
2. **Idempotent:** $\mathbf{P^2 = P}$ *(Projecting twice changes nothing).*
- If $b \in C(A) \implies P b = b$.
- If $b \perp C(A)$ ($b \in N(A^T)$) $\implies P b = \mathbf{0}$.
- **Complementary projection** onto $N(A^T)$: $I - P$ (also symmetric & idempotent).

---

## 5. Least Squares & The Normal Equations ⭐⭐⭐⭐⭐
When $Ax = b$ has no exact solution ($b \notin C(A)$, usually $m > n$), find $\hat{x}$ minimizing squared error $E = \|Ax - b\|^2$.

- Geometry: The error $e = b - A\hat{x}$ must be perpendicular to $C(A)$ $\implies A^T(b - A\hat{x}) = \mathbf{0}$.
- **The Normal Equations (Solve this on exams!):**
  $$\mathbf{A^T A \hat{x} = A^T b}$$
- **Optimal Estimate:** $\mathbf{\hat{x} = (A^T A)^{-1} A^T b}$.
- **Best Projection:** $p = A \hat{x}$.
- **Error Vector:** $e = b - p \in N(A^T)$ ($A^T e = \mathbf{0}$).
- **Invertibility of $A^TA$:** $A^T A$ is invertible $\iff$ columns of $A$ are **linearly independent** ($r = n$).

---

## 6. Orthonormal Bases & Matrices ($Q$)
Vectors $q_1, \dots, q_n$ are **orthonormal** if mutually perpendicular and unit length:

$$q_i^T q_j = \begin{cases} 0 & \text{if } i \ne j \\ 1 & \text{if } i = j \end{cases}$$

- **Matrix with orthonormal columns:**
  $$\mathbf{Q^T Q = I}$$
- **If $Q$ is square ($n \times n$):** It is an **orthogonal matrix**:
  $$\mathbf{Q^{-1} = Q^T} \quad \text{(Inverse is just the transpose!)}$$
- **Preserves geometry:** $\|Qx\| = \|x\|$ and $(Qx)^T (Qy) = x^T y$.
- **Projection onto $C(Q)$ becomes trivial:**
  $$P = Q(Q^T Q)^{-1} Q^T = \mathbf{Q Q^T} \quad \text{and} \quad \hat{x} = Q^T b$$

---

## 7. The Gram-Schmidt Process (Make Vectors Orthonormal)
Converts independent vectors $\{a, b, c\}$ into orthonormal vectors $\{q_1, q_2, q_3\}$:

1. **First direction:**
   $$\mathbf{A = a}$$
2. **Second direction (subtract projection along $A$):**
   $$\mathbf{B = b - \frac{A^T b}{A^T A} A}$$
3. **Third direction (subtract projections along $A$ and $B$):**
   $$\mathbf{C = c - \frac{A^T c}{A^T A} A - \frac{B^T c}{B^T B} B}$$
4. **Normalize to unit vectors:**
   $$\mathbf{q_1 = \frac{A}{\|A\|}}, \quad \mathbf{q_2 = \frac{B}{\|B\|}}, \quad \mathbf{q_3 = \frac{C}{\|C\|}}$$

---

## 8. Key Formulas Quick Table

| Concept | Formula | Key Meaning |
| :--- | :--- | :--- |
| **Orthogonality** | $x^T y = 0$ | Dot product is zero ($90^\circ$ angle) |
| **Line Projection** | $p = a \frac{a^Tb}{a^Ta} = Pb$ | $P = \frac{aa^T}{a^Ta}$ |
| **General Projection** | $P = A(A^TA)^{-1}A^T$ | $P^T = P, \ P^2 = P$ |
| **Normal Equations** | $A^TA\hat{x} = A^Tb$ | Least-squares fitting |
| **Orthonormal Matrix** | $Q^TQ = I$ | $Q^{-1} = Q^T$ (if square) |
| **Projection with $Q$** | $P = QQ^T$ | Denominator $(Q^TQ)^{-1} = I$ drops out |

---

## ⚠️ Chapter 3 Morning Traps to Avoid
1. **$a^Ta$ vs. $aa^T$:** $a^Ta$ is a **scalar** (number); $aa^T$ is an $n \times n$ **matrix**! Never divide a matrix by another matrix.
2. **Do NOT expand $(A^TA)^{-1}$ as $A^{-1}(A^T)^{-1}$:** If $A$ is rectangular ($m \times n$), $A^{-1}$ does NOT exist! $A^TA$ is inverted as a whole block.
3. **$Q^{-1} = Q^T$ is ONLY for SQUARE matrices:** If $Q$ is rectangular ($m > n$), $Q^TQ = I_{n\times n}$, but $QQ^T \ne I$ ($QQ^T = P$, the projection matrix).
4. **Gram-Schmidt Normalization:** Never forget to divide by vector lengths at the end ($q_i = \frac{A_i}{\|A_i\|}$)!
5. **Error Vector Test:** Always check $A^Te = \mathbf{0}$ to verify your least-squares answer in an exam.
