

#ifndef LOB_COMPONENTS_ANIMATION_H
#define LOB_COMPONENTS_ANIMATION_H

/* Animation. The beast of all. Difficult to make cleverly, impossible
 * to make perfectly?
 *
 * In GZigZag the animation was implemented to support changing
 * between views. This was understandable because of the views were
 * more or less cell based. The cell meaning one information item.
 *
 * In QLM there's state based animation. Animations are bound to
 * state, atrribute (e.g. x, y, scale), or timer, or such. There's
 * something very nice in this approach.
 *
 * In libvob the animation has been done traditionally between scenes.
 * If there's animation for each frame, it's harder to do
 * (libvob)scene based animation. For instance animated gif may need
 * frequent animation and there isn't different scene all the
 * time. Well, this is not a big problem actually but in QML it's
 * possible to put like two balls jump in different intervals. What
 * they have decided is more like: when this changes, there's need for
 * next frame.
 *
 * In libvob the matcher made the animation between coordinate
 * systems. But Layoutable objects are higher abstraction than
 * scene. I would like to think that QML like animations can be
 * implemented on top of layoutable object system. Will it be a
 * success, I can't say. The problem between ideas is that lobs come
 * and go, there isn't long standing object while in QML objects are
 * existing like rocks in forest and they are easy to make follow
 * things.
 *
 * What ever is the case, the interstate, state between animations,
 * may be problematic. How are the input events handled, is there
 * model for such?
 *
 * Actually, the (data) model is different than the rendered visual
 * content. Thus, it is easy to implement QML like animation by
 * attaching animation objects to data. Then we could create the lob
 * from data model item. It could be even better than what QML
 * provides because of binding to visual object.
 */

/* The design. By following my nose I will implement this by need. 
 */

/* Hmm.. still wondering. QML has pretty much static scene and
 * animations happen within changes of that scene but not always. 
 * Here's an example of the QML animation documentation:
 *
 *   SequentialAnimation {
 *        id: playbanner
 *        running: false
 *        NumberAnimation { target: code; property: "opacity"; to: 1.0; duration: 200}
 *        NumberAnimation { target: create; property: "opacity"; to: 1.0; duration: 200}
 *        NumberAnimation { target: deploy; property: "opacity"; to: 1.0; duration: 200}
 *    }
 *
 * It means that "Code less." fades in, then "Create more." fades in,
 * and last "Deploy everywhere." fades in. Sequentially. One scene and
 * properties are changed. In libvob this would be done
 * differently. The animation would need switching vob scenes like
 * rerendering because of there's no way to update just a property. I
 * think we should add animation like we do have models.
 * Something like this:
 * 
 *  self.seqAnim = SeqAnim([
 *                    PropAnim(Lob.opacity, to=1.0, ms=200),
 *                    PropAnim(Lob.opacity, to=1.0, ms=200),
 *                    PropAnim(Lob.opacity, to=1.0, ms=200),
 *                 ])
 *  texts = 'Code less.^Create more.^Deploy everywhere.'.split('^')
 *  for t in texts: 
 *    l = Text(t)
 *    l = Anim(l, self.seqAnim[texts.index(t)])
 *    l = Key(l, "mytext"+texts.index(t))
 *
 * Then we could also have AnimIn and AnimOut where animation would be
 * provided between scenes. 
 *
 * ------s1---------------s2-------time
 *  animIn animOut animIn    
 *
 * Or:
 * ------s1---------------s2-------time
 *          animOut for s1
 *           animIn for s2    
 *
 * Different models:
 * - Half, t[0..0.5) Out, t[0.5, 1] In, default
 * - Full, t[0..1)
 * - Empty, t 0 Out, or t 1 In
 * - Pop, t[0.8..1] In 
 * - Shade In/Out, opacity
 *
 * That would be enhancement to QML. Anim models are just
 * reimplementation.  How about putting anim model to this? The item
 * maybe different lob and that's why it may be problematic to say
 * old.shadeOut(), new.shadeIn()
 * 
 */

/* Pondering around, still. I think I got the best solution after
 * all. Ok, first thing, scene based animations can be done but I'm
 * not currently interested in to implement that. It has its uses but
 * my vision of fenfire does not have that many use cases for view
 * changes. View is visualization of data and there might not be many
 * best cases for one user. I will revisit this when I code
 * spreadsheet app ;)
 *
 * Anyway, back to the main issue. I think key based animation still
 * counts, big time. The idea being we mark a key in scene and when
 * scene changes we find out xy (, rotation) and scale. Then key
 * animation model modifies the render while "view changes". Key'ed
 * coordinates are still enhancement to QML.
 *
 * If someone reads this text later one *might* notice that I'm always
 * considering how QML or other does things. The point is, this is
 * kind of research of better, simpler, smaller system. Second thing
 * being I don't want to rewrite things just for nothing. QML bases on
 * Qt and I don't like some of the features of it, e.g. it's way too
 * slow to compile. Nagging, I don't need to compile when I use it over
 * python. 
 */


#include "lob/api.h"
#include "lob/models/keyanimmodel.h"

struct lob_key {
	LobDelegate base;
	Size size;
	char *key;
};
typedef struct lob_key LobKey;
Lob *lob_key(Region *reg, Lob *delegate, char *key);


struct lob_key_anim {
	LobDelegate base;
	Size size;
	char *key;
	bool make_key;
	LobKeyAnimModel *model;
};
typedef struct lob_key_anim LobKeyAnim;
Lob *lob_key_anim(Region *reg, Lob *delegate, char *key, bool actAsKey, LobKeyAnimModel *m);


#endif
