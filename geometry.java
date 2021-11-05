import java.util.*;
import static java.lang.Math.*;

public class geometry {
	static boolean e(double a, double b) {return abs(a-b)<1e-9;}
	static class pt implements Comparable<pt>{
		double x, y;
		public pt(double a, double b) {x=a;y=b;}
		pt polar(double a, double b) {return new pt(cos(b) * a, sin(b) * a);}
		pt add(pt o, double scl) {return new pt(x+o.x*scl,y+o.y*scl);}
		pt scl(double scl) {return new pt(x * scl, y*scl);}
		pt mult(pt b) {return new pt(x * b.x - y * b.y,x * b.y + y* b.x);}
		double abs() {return sqrt(x*x+y*y);}
		double arg() {return atan(y/x);}
		pt conj(pt a) { return new pt(x,-y);}
		boolean eq(pt o){return e(x,o.x) && e(y,o.y);}
		public int compareTo(pt o) {
			if (e(x,o.x)) {
				if (e(y,o.y)) return 0;
				if (y<o.y) return -1;
				return 1;
			}
			if (x < o.x) return -1;
			return 1;
		}
	}
	static class line{
		pt P, D; // L = P + Dt (0<=t<=1 for seg). For a ray, do a REALLY long segment.
		boolean S; // isSegment
		double slope;
		public line(pt a, pt b, boolean c) {
			P=a;
			D=b.add(a, -1);
			S=c;
			slope = (b.y-a.y)/(b.x-a.x);
		}
		boolean onl(pt Q) {return !S || DOT(Q.add(P, -1), Q.add(P.add(D, 1), -1)) <= 0;}
		boolean ints(line o) {
			double t = CRS(o.D, o.P.add(P, -1))/CRS(o.D,D);
			if (!Double.isFinite(t)) return false;
			pt Q = P.add(D,t);
			return onl(Q) && o.onl(Q);
		}
	}
	static double CRS(pt a, pt b){return a.conj(a).mult(b).y;}
	static double DOT(pt a, pt b) {return a.conj(a).mult(b).x;}
	static double area(List<pt> a) {
		double ans = 0;
		pt last = a.get(a.size()-1);
		for (pt p: a) {
			ans += CRS(last,p);
			last = p;
		}
		return abs(ans/2);
	}
	static List<pt> getHull(List<pt> pts) {
		List<pt> top = new ArrayList<pt>(), bot = new ArrayList<pt>();
		Collections.sort(pts);
		doHull(pts,top);
		Collections.reverse(pts);
		doHull(pts,bot);
		top.addAll(bot);
		return top;
	}
	static void doHull(List<pt> pts, List<pt> h) {
		for (pt p: pts) {
			while (h.size() > 1 && CRS(h.get(h.size()-1).add(p, -1), h.get(h.size()-2).add(p, -1)) <= 0) h.remove(h.size()-1);
			h.add(p);
		}
	}
	
}
