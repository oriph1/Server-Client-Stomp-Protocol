package bgu.spl.net.impl.stomp;

import java.util.Objects;

public class Triple<L,M,R> {
    public L left;
    public R right;
    public M middle;


    public Triple(L left,M middle,R right) {
        this.left = left;
        this.right = right;
        this.middle=middle;
    }

    public L getLeft() {
        return this.left;
    }

    public R getRight() {
        return this.right;
    }
    public M getMiddle() {
        return this.middle;
    }

    public int hashCode() {
        return Objects.hashCode(this.left) + 31 * Objects.hashCode(this.right) + 37*Objects.hashCode(this.middle);
    }

    public boolean equals(Object obj) {
        if (obj == this) {
            return true;
        }
        else {
            return Objects.equals(this.left, ((Pair)obj).getLeft()) && Objects.equals(this.right, ((Pair)obj).getRight());
        }
    }

    public String toString() {
        return String.format("(%s, %s, %s)", this.left,this.middle, this.right);
    }
}
