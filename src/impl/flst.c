#include "impl_mlc.h"

void	_insertFlst(t_flst *_pFree, t_heap *_heap) {
	t_flst	*fwd = _heap->flst;
	t_flst	*bck = NULL;

	while (fwd != NULL && fwd < _pFree) {
		bck = fwd;
		fwd = fwd->fwd;
	}
	
	if (fwd != NULL) {
		fwd->bck = _pFree;
	}
	_pFree->fwd = fwd;
	_pFree->bck = bck;
	if (bck != NULL) {
		bck->fwd = _pFree;
	} else {
		_heap->flst = _pFree;
	}
}
